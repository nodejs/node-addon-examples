#include <node_jsvmapi.h>

void Method(napi_env napi_env, napi_func_cb_info info) {
    napi_set_return_value(
        napi_env,
        info,
        napi_create_string(napi_env, "world"));
}

void new_init(napi_env napi_env, napi_value exports, napi_value module) {
  napi_set_property(napi_env, exports,
                        napi_proterty_name(napi_env, "hello"),
                        napi_create_function(napi_env, Method));
}


// NODE_MODULE's init napi_callback's signature uses v8 type for its parameter
// // which complicates our effort to create a VM neutral and ABI stable API.
// // For not I am working around the issue by assuming v8 and thunking into
// // an init napi_callback with a VM neutral signature.
void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  WorkaroundNewModuleInit(exports, module, new_init);
}

NODE_MODULE(addon, Init)

