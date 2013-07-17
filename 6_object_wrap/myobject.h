#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <node.h>

class MyObject : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  MyObject();
  ~MyObject();

  template<class T> static void New(const v8::FunctionCallbackInfo<T>& info);
  template<class T> static void PlusOne(const v8::FunctionCallbackInfo<T>& info);
  double counter_;
};

#endif
