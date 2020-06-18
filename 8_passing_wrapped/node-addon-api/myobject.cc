#include "myobject.h"
#include <napi.h>
#include <uv.h>

MyObject::MyObject(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<MyObject>(info) {

  this->val_ = info[0].As<Napi::Number>().DoubleValue();
};

Napi::FunctionReference MyObject::constructor;

void MyObject::Init(Napi::Env env, Napi::Object exports) {

  Napi::Function func = DefineClass(env, "MyObject", {});

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("MyObject", func);
}

Napi::Object MyObject::NewInstance(Napi::Value arg) {
  Napi::Object obj = constructor.New({arg});
  return obj;
}
