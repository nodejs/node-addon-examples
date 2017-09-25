#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <napi.h>

class MyObject : public Napi::ObjectWrap<MyObject> {
 public:
  static void Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Value arg);
  double Val() const { return val_; }
  MyObject(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;
  double val_;
};

#endif
