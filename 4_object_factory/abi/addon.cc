#include <node_jsvmapi.h>

void napi_create_object(napi_env env, const napi_func_cb_info info) {
  napi_value args[1];
  napi_get_cb_args(env, info, args, 1);

  napi_value obj = napi_create_object(env);
  napi_set_property(env, obj, napi_property_name(env, "msg"),
                        args[0]);

  napi_set_return_value(env, info, obj);
}

void Init(napi_env env, napi_value exports, napi_value module) {
  napi_set_property(env, module,
                        napi_property_name(env, "exports"),
                        napi_create_function(env, napi_create_object));
}

NODE_MODULE_ABI(addon, Init)
