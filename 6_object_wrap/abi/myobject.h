#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <node_jsvmapi.h>

class MyObject {
 public:
  static void Init(node::js::value env, node::js::value exports);
  static void Destructor(void* jsObject);

 private:
  explicit MyObject(double value = 0);
  ~MyObject();

  static void New(node::js::value env, node::js::FunctionCallbackInfo info);
  static void GetValue(node::js::value env, node::js::FunctionCallbackInfo info);
  static void PlusOne(node::js::value env, node::js::FunctionCallbackInfo info);
  static void Multiply(node::js::value env, node::js::FunctionCallbackInfo info);
  static node::js::persistent constructor;
  double value_;
};

#endif
