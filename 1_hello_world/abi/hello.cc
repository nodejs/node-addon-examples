#include <node_jsvmapi.h>

void Method(napi_env napi_env, napi_func_cb_info info) {
  napi_set_return_value(
        napi_env,
        info,
        napi_create_string(napi_env, "world"));
}

void Init(napi_env napi_env, napi_value exports, napi_value module) {
  napi_set_property(napi_env, exports,
                    napi_property_name(napi_env, "hello"),
                    napi_create_function(napi_env, Method));
}

NODE_MODULE_ABI(addon, Init)

