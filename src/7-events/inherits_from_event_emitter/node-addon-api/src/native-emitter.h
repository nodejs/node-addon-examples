#include <napi.h>

class NativeEmitter : public Napi::ObjectWrap<NativeEmitter> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  NativeEmitter(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;

  Napi::Value CallAndEmit(const Napi::CallbackInfo& info);
};
