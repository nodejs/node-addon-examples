#include <napi.h>

class HelloAddon : public Napi::Addon<HelloAddon> {
 public:
  HelloAddon(Napi::Env env, Napi::Object exports) {
    DefineAddon(exports,
                {InstanceMethod("hello", &HelloAddon::Hello, napi_enumerable)});
  }

 private:
  Napi::Value Hello(const Napi::CallbackInfo& info) {
    return Napi::String::New(info.Env(), "world");
  }
};

NODE_API_ADDON(HelloAddon)
