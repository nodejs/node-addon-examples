#include "native-addon.h"
#include <iostream>

Napi::FunctionReference NativeAddon::constructor;

Napi::Object NativeAddon::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);
  
  Napi::Function func = DefineClass(env, "NativeAddon", {
    InstanceMethod("tryCallByStoredReference", &NativeAddon::TryCallByStoredReference),
    InstanceMethod("tryCallByStoredFunction", &NativeAddon::TryCallByStoredFunction)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("NativeAddon", func);
  return exports;
}

NativeAddon::NativeAddon(const Napi::CallbackInfo& info) 
: Napi::ObjectWrap<NativeAddon>(info)  {
  jsFnRef = Napi::Persistent(info[0].As<Napi::Function>());
  jsFn = info[1].As<Napi::Function>();
}

void NativeAddon::TryCallByStoredReference(const Napi::CallbackInfo& info) {
    // Napi::Env env = info.Env();
    jsFnRef.Call({});
}

void NativeAddon::TryCallByStoredFunction(const Napi::CallbackInfo& info) {
    // Napi::Env env = info.Env();
    jsFn.Call({});
}