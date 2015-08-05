#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <nan.h>

class MyObject : public node::ObjectWrap {
 public:
  static NAN_MODULE_INIT(Init);

 private:
  explicit MyObject(double value = 0);
  ~MyObject();

  static NAN_METHOD(New);
  static NAN_METHOD(PlusOne);
  static Nan::Persistent<v8::Function> constructor;
  double value_;
};

#endif
