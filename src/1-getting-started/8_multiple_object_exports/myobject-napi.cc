#include "myobject-napi.h"

MyObjectNapi::MyObjectNapi(const Napi::CallbackInfo& info) : Napi::ObjectWrap<MyObjectNapi>(info) {
  if ( info.Length() > 0 && info[0].IsNumber() )
    _nativeMyObject = std::make_shared<MyObject>(info[0].As<Napi::Number>().DoubleValue());
  else
    _nativeMyObject = std::make_shared<MyObject>(0);
};

void MyObjectNapi::init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "MyObjectNapi", {
    InstanceMethod("multiply", &MyObjectNapi::multiply),
    InstanceMethod("plusOne", &MyObjectNapi::plusOne),
    InstanceMethod("value", &MyObjectNapi::value)
  });
  
  exports.Set("MyObject", func);
}

Napi::Value MyObjectNapi::multiply(const Napi::CallbackInfo& info) {
  if ( info.Length() < 1 || !info[0].IsNumber() )
    throw Napi::Error::New(info.Env(), "Invalid argument");
    
  return Napi::Number::New(info.Env(), _nativeMyObject->multiply(info[0].As<Napi::Number>().DoubleValue()));
}

Napi::Value MyObjectNapi::plusOne(const Napi::CallbackInfo& info) {
  return Napi::Number::New(info.Env(), _nativeMyObject->plusOne());
}

Napi::Value MyObjectNapi::value(const Napi::CallbackInfo& info) {
  if ( info.Length() == 0 )
    return Napi::Number::New(info.Env(), _nativeMyObject->value());
  
  _nativeMyObject->value(info[0].As<Napi::Number>().DoubleValue());
  
  return info.Env().Null();
}
