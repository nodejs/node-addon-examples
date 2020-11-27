#include <napi.h>

#include <chrono>
#include <iostream>
#include <thread>

Napi::Value CallEmit(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Function emit = info[0].As<Napi::Function>();
  emit.Call({Napi::String::New(env, "start")});
  for (int i = 0; i < 3; i++) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    emit.Call(
        {Napi::String::New(env, "data"), Napi::String::New(env, "data ...")});
  }
  emit.Call({Napi::String::New(env, "end")});
  return Napi::String::New(env, "OK");
}

// Init
Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "callEmit"),
              Napi::Function::New(env, CallEmit));
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init);
