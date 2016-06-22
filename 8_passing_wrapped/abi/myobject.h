#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <node_jsvmapi.h>

class MyObject {
 public:
  static void Init(node::js::value env);
  static void Destructor(void* nativeObject);
  static node::js::value NewInstance(node::js::value env, node::js::value arg);
  double Val() const { return val_; }

 private:
  MyObject();
  ~MyObject();

  static node::js::persistent constructor;
  static void New(node::js::value env, node::js::FunctionCallbackInfo info);
  double val_;
};

#endif
