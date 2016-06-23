#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <node_jsvmapi.h>

class MyObject {
 public:
  static void Init(napi_env env, napi_value exports);
  static void Destructor(void* nativeObject);

 private:
  explicit MyObject(double value_ = 0);
  ~MyObject();

  static void New(napi_env env, napi_func_cb_info info);
  static void GetValue(napi_env env, napi_func_cb_info info);
  static void PlusOne(napi_env env, napi_func_cb_info info);
  static void Multiply(napi_env env, napi_func_cb_info info);
  static napi_persistent constructor;
  double value_;
};

#endif
