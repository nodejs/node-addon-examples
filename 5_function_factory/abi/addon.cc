#include <node_jsvmapi.h>

void MyFunction(napi_env napi_env, napi_func_cb_info info) { 
  napi_set_return_value(napi_env, info, napi_create_string(napi_env, "hello world"));
}

void napi_create_function(napi_env napi_env, napi_func_cb_info info) { 
  napi_value fn = napi_create_function(napi_env, MyFunction);

  // omit this to make it anonymous
  napi_set_function_name(napi_env, fn, napi_create_string(napi_env, "theFunction"));

  napi_set_return_value(napi_env, info, fn);
}

void Init(napi_env napi_env, napi_value exports, napi_value module) {
  napi_set_property(napi_env, module,
                        napi_proterty_name(napi_env, "exports"),
                        napi_create_function(napi_env, napi_create_function));
}

NODE_MODULE_ABI(addon, Init)

