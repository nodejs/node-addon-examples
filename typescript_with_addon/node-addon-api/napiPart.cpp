#include <napi.h>

extern int callingCPart(int value);

Napi::Value Method(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  int value = info[0].As<Napi::Number>().Int32Value();
  Napi::Number num = Napi::Number::New(env, callingCPart(value));
  return num;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "cppPartExportedByNapi"),
              Napi::Function::New(env, Method));
  return exports;
}

NODE_API_MODULE(cppPartExportedByNapi, Init)