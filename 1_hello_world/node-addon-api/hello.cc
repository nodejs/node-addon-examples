#include <napi.h>
#include <uv.h>

Napi::String Method(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, "world");
}

void Init(Napi::Env env, Napi::Object exports, Napi::Object module) {
  exports.Set(Napi::String::New(env, "hello"),
              Napi::Function::New(env, Method));
}

NODE_API_MODULE(hello, Init)
