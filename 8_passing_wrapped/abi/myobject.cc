#include <node.h>
#include "myobject.h"


MyObject::MyObject() {};
MyObject::~MyObject() {};

void MyObject::Destructor(void* nativeObject) {
  ((MyObject*) nativeObject)->~MyObject();
}

napi_persistent MyObject::constructor;

void MyObject::Init(napi_env env) {
  napi_value function = napi_create_constructor_for_wrap(env, New);
  napi_set_function_name(env, function, napi_create_string(env, "MyObject"));

  constructor = napi_create_persistent(env, function);
}

void MyObject::New(napi_env env, napi_func_cb_info info) {
  napi_value args[1];
  napi_get_cb_args(env, info, args, 1);
  MyObject* obj = new MyObject();
  obj->val_ = (args[0] == napi_get_undefined_(env)) ? 0 : napi_get_number_from_value(env, args[0]);
  napi_value jsobj = napi_get_cb_object(env, info);
  napi_wrap(env, jsobj, (void*) obj, MyObject::Destructor);
  napi_set_return_value(env, info, jsobj);
}

napi_value MyObject::NewInstance(napi_env env, napi_value arg) {
  const int argc = 1;
  napi_value argv[argc] = { arg };
  napi_value cons = napi_get_persistent_value(env, constructor);
  return napi_new_instance(env, cons, argc, argv);
}
