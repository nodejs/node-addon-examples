#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <nan.h>

class MyObject : public node::ObjectWrap {
 public:
  static void Init();
  static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> arg);

 private:
  MyObject();
  ~MyObject();

  static v8::Persistent<v8::Function> constructor;
  static NAN_METHOD(New);
  static NAN_METHOD(PlusOne);
  double counter_;
};

#endif
