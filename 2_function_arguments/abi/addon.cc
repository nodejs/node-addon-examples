#include <node_jsvmapi.h>

void Add(napi_env napi_env, napi_func_cb_info info) {
    if (napi_get_cb_args_length(napi_env, info) < 2) {
        napi_throw_error(
            napi_env,
            napi_create_type_error(
                napi_env,
                napi_create_string(napi_env, "Wrong number of arguments")));
        return;
    }

    napi_value args[2];
    napi_get_cb_args(napi_env, info, args, 2);

    if (napi_get_type_of_value(napi_env, args[0]) != napi_valuetype::Number ||
        napi_get_type_of_value(napi_env, args[1]) != napi_valuetype::Number) {
        napi_throw_error(
            napi_env,
            napi_create_type_error(
                napi_env,
                napi_create_string(napi_env, "Wrong arguments")));
        return;
    }

    double value = napi_get_number_from_value(napi_env, args[0]) + napi_get_number_from_value(napi_env, args[1]);
    napi_value num = napi_create_number(napi_env, value);

    napi_set_return_value(napi_env, info, num);
}


void new_init(napi_env napi_env, napi_value exports, napi_value module) {
  napi_set_property(napi_env, exports,
                        napi_proterty_name(napi_env, "add"),
                        napi_create_function(napi_env, Add));
}

// NODE_MODULE's init napi_callback's signature uses v8 type for its parameter
// // which complicates our effort to create a VM neutral and ABI stable API.
// // For not I am working around the issue by assuming v8 and thunking into
// // an init napi_callback with a VM neutral signature.
void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  WorkaroundNewModuleInit(exports, module, new_init);
}

NODE_MODULE(addon, Init)
