#include <chrono>
#include <iostream>
#include <thread>

#include "native-emitter.h"

Napi::FunctionReference NativeEmitter::constructor;

Napi::Object NativeEmitter::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func =
      DefineClass(env,
                  "NativeEmitter",
                  {InstanceMethod("callAndEmit", &NativeEmitter::CallAndEmit)});

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("NativeEmitter", func);
  return exports;
}

NativeEmitter::NativeEmitter(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<NativeEmitter>(info) {
  // NOOP
}

Napi::Value NativeEmitter::CallAndEmit(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Function emit =
      info.This().As<Napi::Object>().Get("emit").As<Napi::Function>();
  emit.Call(info.This(), {Napi::String::New(env, "start")});
  for (int i = 0; i < 3; i++) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    emit.Call(
        info.This(),
        {Napi::String::New(env, "data"), Napi::String::New(env, "data ...")});
  }
  emit.Call(info.This(), {Napi::String::New(env, "end")});
  return Napi::String::New(env, "OK");
}
