#include <napi.h>
#include <uv.h>

Napi::Value Method(const Napi::CallbackInfo& info) {
  return Napi::String::New(info.Env(), "world");
}

void Init(Napi::Env env, Napi::Object exports, Napi::Object module) {
  exports.Set("hello", Napi::Function::New(env, Method));
}

NODE_API_MODULE(hello, Init)
