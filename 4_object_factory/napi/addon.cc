#include <node_api.h>
#include <assert.h>

napi_value CreateObject(napi_env env, const napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value args[1];
  status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
  assert(status == napi_ok);

  napi_value obj;
  status = napi_create_object(env, &obj);
  assert(status == napi_ok);

  status = napi_set_named_property(env, obj, "msg", args[0]);
  assert(status == napi_ok);

  return obj;
}

napi_value Init(napi_env env, napi_value exports) {
  napi_value new_exports;
  napi_status status =
      napi_create_function(env, "", NAPI_AUTO_LENGTH, CreateObject, nullptr, &new_exports);
  assert(status == napi_ok);
  return new_exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
