#include <node_api.h>
#include <assert.h>

void Add(napi_env env, napi_callback_info info) {
  napi_status status;

  int argc;
  status = napi_get_cb_args_length(env, info, &argc);
  assert(status == napi_ok);

  if (argc < 2) {
    napi_throw_type_error(env, "Wrong number of arguments");
    return;
  }

  napi_value args[2];
  status = napi_get_cb_args(env, info, args, 2);
  assert(status == napi_ok);

  napi_valuetype valuetype0;
  status = napi_get_type_of_value(env, args[0], &valuetype0);
  assert(status == napi_ok);

  napi_valuetype valuetype1;
  status = napi_get_type_of_value(env, args[1], &valuetype1);
  assert(status == napi_ok);

  if (valuetype0 != napi_number || valuetype1 != napi_number) {
    napi_throw_type_error(env, "Wrong arguments");
    return;
  }

  double value0;
  status = napi_get_value_double(env, args[0], &value0);
  assert(status == napi_ok);

  double value1;
  status = napi_get_value_double(env, args[1], &value1);
  assert(status == napi_ok);

  napi_value sum;
  status = napi_create_number(env, value0 + value1, &sum);
  assert(status == napi_ok);

  status = napi_set_return_value(env, info, sum);
  assert(status == napi_ok);
}

#define DECLARE_NAPI_METHOD(name, func)                          \
  { name, func, 0, 0, 0, napi_default, 0 }

void Init(napi_env env, napi_value exports, napi_value module, void* priv) {
  napi_status status;
  napi_property_descriptor addDescriptor = DECLARE_NAPI_METHOD("add", Add);
  status = napi_define_properties(env, exports, 1, &addDescriptor);
  assert(status == napi_ok);
}

NAPI_MODULE(addon, Init)
