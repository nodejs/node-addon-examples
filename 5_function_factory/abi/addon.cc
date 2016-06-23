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

void new_init(napi_env napi_env, napi_value exports, napi_value module) {
  napi_set_property(napi_env, module,
                        napi_proterty_name(napi_env, "exports"),
                        napi_create_function(napi_env, napi_create_function));
}

// NODE_MODULE's init napi_callback's signature uses v8 type for its parameter
// // which complicates our effort to create a VM neutral and ABI stable API.
// // For not I am working around the issue by assuming v8 and thunking into
// // an init napi_callback with a VM neutral signature.
void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  WorkaroundNewModuleInit(exports, module, new_init);
}

NODE_MODULE(addon, Init)

