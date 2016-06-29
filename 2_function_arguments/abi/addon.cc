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

  if (napi_get_type_of_value(napi_env, args[0]) != napi_number ||
    napi_get_type_of_value(napi_env, args[1]) != napi_number) {
    napi_throw_error(
        napi_env,
        napi_create_type_error(
             napi_env,
             napi_create_string(napi_env, "Wrong arguments")));
    return;
  }

  double value = napi_get_number_from_value(napi_env, args[0])
               + napi_get_number_from_value(napi_env, args[1]);
  napi_value num = napi_create_number(napi_env, value);

  napi_set_return_value(napi_env, info, num);
}


void Init(napi_env napi_env, napi_value exports, napi_value module) {
  napi_set_property(napi_env, exports,
                    napi_proterty_name(napi_env, "add"),
                    napi_create_function(napi_env, Add));
}

NODE_MODULE_ABI(addon, Init)
