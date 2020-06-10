#include <napi.h>
#include "myobject.h"

using namespace Napi;

Napi::Object CreateObject(const Napi::CallbackInfo& info) {
  return MyObject::NewInstance(info.Env(), info[0]);
}

Napi::Number Add(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  MyObject* obj1 =
      Napi::ObjectWrap<MyObject>::Unwrap(info[0].As<Napi::Object>());
  MyObject* obj2 =
      Napi::ObjectWrap<MyObject>::Unwrap(info[1].As<Napi::Object>());
  double sum = obj1->Val() + obj2->Val();
  return Napi::Number::New(env, sum);
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  MyObject::Init(env, exports);

  exports.Set(Napi::String::New(env, "createObject"),
              Napi::Function::New(env, CreateObject));

  exports.Set(Napi::String::New(env, "add"), Napi::Function::New(env, Add));

  return exports;
}

NODE_API_MODULE(addon, InitAll)
