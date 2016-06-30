#include <node_jsvmapi.h>

void RunCallback(napi_env napi_env, const napi_func_cb_info info) {
  napi_value args[1];
  napi_get_cb_args(napi_env, info, args, 1);
  napi_value cb = args[0];

  napi_value argv[1];
  argv[0] = napi_create_string(napi_env, "hello world");
  napi_call_function(napi_env, napi_get_global_scope(napi_env) , cb, 1, argv);
}

void Init(napi_env napi_env, napi_value exports, napi_value module) {
  napi_set_property(napi_env, module,
                        napi_property_name(napi_env, "exports"),
                        napi_create_function(napi_env, RunCallback));
}

NODE_MODULE_ABI(addon, Init)
