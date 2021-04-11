#include "object_wrap_demo.h"

using namespace Napi;

ObjectWrapDemo::ObjectWrapDemo(const Napi::CallbackInfo& info)
    : ObjectWrap(info) {
  Env env = info.Env();

  if (info.Length() < 1) {
    TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  if (!info[0].IsString()) {
    TypeError::New(env, "You need to name yourself")
        .ThrowAsJavaScriptException();
    return;
  }

  this->_greeterName = info[0].As<String>().Utf8Value();
}

Value ObjectWrapDemo::Greet(const CallbackInfo& info) {
  Env env = info.Env();

  if (info.Length() < 1) {
    TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString()) {
    TypeError::New(env, "You need to introduce yourself to greet")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  String name = info[0].As<String>();

  printf("Hello %s\n", name.Utf8Value().c_str());
  printf("I am %s\n", this->_greeterName.c_str());

  return String::New(env, this->_greeterName);
}

Function ObjectWrapDemo::GetClass(Env env) {
  return DefineClass(
      env,
      "ObjectWrapDemo",
      {
          ObjectWrapDemo::InstanceMethod("greet", &ObjectWrapDemo::Greet),
      });
}

Object Init(Env env, Object exports) {
  String name = String::New(env, "ObjectWrapDemo");
  exports.Set(name, ObjectWrapDemo::GetClass(env));
  return exports;
}

NODE_API_MODULE(addon, Init)
