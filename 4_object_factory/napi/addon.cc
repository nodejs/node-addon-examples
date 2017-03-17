#include <node_api.h>
#include <assert.h>

void CreateObject(napi_env env, const napi_callback_info info) {
  napi_status status;

  napi_value args[1];
  status = napi_get_cb_args(env, info, args, 1);
  assert(status == napi_ok);

  napi_value obj;
  status = napi_create_object(env, &obj);
  assert(status == napi_ok);

  status = napi_set_named_property(env, obj, "msg", args[0]);
  assert(status == napi_ok);

  status = napi_set_return_value(env, info, obj);
  assert(status == napi_ok);
}

#define DECLARE_NAPI_METHOD(name, func)                          \
  { name, func, 0, 0, 0, napi_default, 0 }

void Init(napi_env env, napi_value exports, napi_value module) {
  napi_status status;
  napi_property_descriptor desc = DECLARE_NAPI_METHOD("exports", CreateObject);
  status = napi_define_properties(env, module, 1, &desc);
  assert(status == napi_ok);
}

NAPI_MODULE(addon, Init)
