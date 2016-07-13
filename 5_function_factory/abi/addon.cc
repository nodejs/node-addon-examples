#include <node_jsvmapi.h>

void MyFunction(napi_env env, napi_func_cb_info info) { 
  napi_set_return_value(env, info, napi_create_string(env, "hello world"));
}

void napi_create_function(napi_env env, napi_func_cb_info info) { 
  napi_value fn = napi_create_function(env, MyFunction);

  // omit this to make it anonymous
  napi_set_function_name(env, fn, napi_property_name(env, "theFunction"));

  napi_set_return_value(env, info, fn);
}

void Init(napi_env env, napi_value exports, napi_value module) {
  napi_set_property(env, module,
                        napi_property_name(env, "exports"),
                        napi_create_function(env, napi_create_function));
}

NODE_MODULE_ABI(addon, Init)

