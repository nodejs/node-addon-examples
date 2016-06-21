#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <node_jsvmapi.h>

class MyObject {
 public:
  static void Init(node::js::value env);
  static void Destructor(void* jsObject);
  static node::js::value NewInstance(node::js::value env, node::js::value arg);

 private:
  MyObject();
  ~MyObject();

  static node::js::persistent constructor;
  static void New(node::js::value env, node::js::FunctionCallbackInfo info);
  static void PlusOne(node::js::value env, node::js::FunctionCallbackInfo info);
  double counter_;
};

#endif
