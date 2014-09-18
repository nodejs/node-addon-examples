#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <nan.h>

class MyObject : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  explicit MyObject(double value = 0);
  ~MyObject();

  static NAN_METHOD(New);
  static NAN_METHOD(PlusOne);
  static v8::Persistent<v8::Function> constructor;
  double value_;
};

#endif
