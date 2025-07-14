#include "otherobject-napi.h"

OtherObjectNapi::OtherObjectNapi(const Napi::CallbackInfo& info) : Napi::ObjectWrap<OtherObjectNapi>(info) {
  if ( info.Length() > 0 && info[0].IsString() )
    _nativeOtherObject = std::make_shared<OtherObject>(info[0].As<Napi::String>().Utf8Value());
  else
    _nativeOtherObject = std::make_shared<OtherObject>("");
};

void OtherObjectNapi::init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "OtherObjectNapi", {
    InstanceMethod("multiply", &OtherObjectNapi::multiply),
    InstanceMethod("append", &OtherObjectNapi::append),
    InstanceMethod("value", &OtherObjectNapi::value)
  });
  
  exports.Set("OtherObject", func);
}

Napi::Value OtherObjectNapi::multiply(const Napi::CallbackInfo& info) {
  if ( info.Length() < 1 || !info[0].IsNumber() )
    throw Napi::Error::New(info.Env(), "Invalid argument");
    
  return Napi::String::New(info.Env(), _nativeOtherObject->multiply(info[0].As<Napi::Number>().Int32Value()));
}

Napi::Value OtherObjectNapi::append(const Napi::CallbackInfo& info) {
  if ( info.Length() < 1 || !info[0].IsString() )
    throw Napi::Error::New(info.Env(), "Invalid argument");
    
  return Napi::String::New(info.Env(), _nativeOtherObject->append(info[0].As<Napi::String>().Utf8Value()));
}

Napi::Value OtherObjectNapi::value(const Napi::CallbackInfo& info) {
  if ( info.Length() == 0 )
    return Napi::String::New(info.Env(), _nativeOtherObject->value());
  
  _nativeOtherObject->value(info[0].As<Napi::String>().Utf8Value());
  
  return info.Env().Null();
}
