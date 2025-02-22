#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <nan.h>

class MyObject : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);

 private:
  explicit MyObject(double value = 0);
  ~MyObject();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void GetValueAcc(v8::Local<v8::String> property,
                          const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void SetValueAcc(v8::Local<v8::String> property,
                          v8::Local<v8::Value> value,
                          const Nan::PropertyCallbackInfo<void>& info);

  static void GetValue(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void PlusOne(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Multiply(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static Nan::Persistent<v8::Function> constructor;
  double value_;
};

#endif
