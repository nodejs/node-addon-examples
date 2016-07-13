#include <node_jsvmapi.h>

void Method(napi_env env, napi_func_cb_info info) {
  napi_set_return_value(
        env,
        info,
        napi_create_string(env, "world"));
}

void Init(napi_env env, napi_value exports, napi_value module) {
  napi_set_property(env, exports,
                    napi_property_name(env, "hello"),
                    napi_create_function(env, Method));
}

NODE_MODULE_ABI(addon, Init)

