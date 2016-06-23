#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <node_jsvmapi.h>

class MyObject {
 public:
  static void Init(napi_env env);
  static void Destructor(void* nativeObject);
  static napi_value NewInstance(napi_env env, napi_value arg);
  double Val() const { return val_; }

 private:
  MyObject();
  ~MyObject();

  static napi_persistent constructor;
  static void New(napi_env env, napi_func_cb_info info);
  double val_;
};

#endif
