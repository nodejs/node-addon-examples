#include <napi.h>

Napi::String MyFunction(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, "hello world");
}

Napi::Function CreateFunction(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Function fn = Napi::Function::New(env, MyFunction, "theFunction");
  return fn;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  return Napi::Function::New(env, CreateFunction, "createObject");
}

NODE_API_MODULE(addon, Init)
