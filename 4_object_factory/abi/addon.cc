#include <node_jsvmapi.h>

void napi_create_object(napi_env napi_env, const napi_func_cb_info info) {
  napi_value args[1];
  napi_get_cb_args(napi_env, info, args, 1);

  napi_value obj = napi_create_object(napi_env);
  napi_set_property(napi_env, obj, napi_proterty_name(napi_env, "msg"),
                        args[0]);

  napi_set_return_value(napi_env, info, obj);
}

void Init(napi_env napi_env, napi_value exports, napi_value module) {
  napi_set_property(napi_env, module,
                        napi_proterty_name(napi_env, "exports"),
                        napi_create_function(napi_env, napi_create_object));
}

NODE_MODULE_ABI(addon, Init)
