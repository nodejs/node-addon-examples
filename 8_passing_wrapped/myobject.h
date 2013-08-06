#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <node.h>

extern v8::Isolate* isolate;

class MyObject : public node::ObjectWrap {
 public:
  static void Init();
  static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& info);
  double Val() const { return val_; }

 private:
  MyObject();
  ~MyObject();

  static v8::Persistent<v8::Function> constructor;
  template<class T> static void New(const v8::FunctionCallbackInfo<T>& info);
  double val_;
};

#endif
