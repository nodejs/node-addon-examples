#include "myobject.h"

void napi_create_object(napi_env env, napi_func_cb_info info) {
  napi_value args[1];
  napi_get_cb_args(env, info, args, 1);
  napi_set_return_value(env, info, MyObject::NewInstance(env, args[0]));
}

void Init(napi_env env, napi_value exports, napi_value module) {
  MyObject::Init(env);
  napi_set_property(env, module,
                        napi_proterty_name(env, "exports"),
                        napi_create_function(env, napi_create_object));
}

NODE_MODULE_ABI(addon, Init)
