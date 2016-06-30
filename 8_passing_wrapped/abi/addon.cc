#include "myobject.h"

void napi_create_object(napi_env env, napi_func_cb_info info) {
  napi_value args[1];
  napi_get_cb_args(env, info, args, 1);
  napi_set_return_value(env, info, MyObject::NewInstance(env, args[0]));
}

void Add(napi_env env, napi_func_cb_info info) {
  napi_value args[2];
  napi_get_cb_args(env, info, args, 2);
  MyObject* obj1 = (MyObject*) napi_unwrap(env, args[0]);
  MyObject* obj2 = (MyObject*) napi_unwrap(env, args[1]);
  double sum = obj1->Val() + obj2->Val();
  napi_set_return_value(env, info, napi_create_number(env,sum));
}

void Init(napi_env env, napi_value exports, napi_value module) {
  MyObject::Init(env);

  napi_set_property(env, exports,
              napi_property_name(env, "createObject"),
              napi_create_function(env, napi_create_object));


  napi_set_property(env, exports,
              napi_property_name(env, "add"),
              napi_create_function(env, Add));
}


NODE_MODULE_ABI(addon, Init)
