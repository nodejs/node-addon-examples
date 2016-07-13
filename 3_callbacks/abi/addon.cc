#include <node_jsvmapi.h>

void RunCallback(napi_env env, const napi_func_cb_info info) {
  napi_value args[1];
  napi_get_cb_args(env, info, args, 1);
  napi_value cb = args[0];

  napi_value argv[1];
  argv[0] = napi_create_string(env, "hello world");
  napi_call_function(env, napi_get_global_scope(env) , cb, 1, argv);
}

void Init(napi_env env, napi_value exports, napi_value module) {
  napi_set_property(env, module,
                        napi_property_name(env, "exports"),
                        napi_create_function(env, RunCallback));
}

NODE_MODULE_ABI(addon, Init)
