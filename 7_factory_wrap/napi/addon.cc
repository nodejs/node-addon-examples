#include "myobject.h"
#include <assert.h>

napi_value CreateObject(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value args[1];
  status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
  assert(status == napi_ok);

  napi_value instance;
  status = MyObject::NewInstance(env, args[0], &instance);
  assert(status == napi_ok);

  return instance;
}

napi_value Init(napi_env env, napi_value exports) {
  napi_status status = MyObject::Init(env);
  assert(status == napi_ok);

  napi_value new_exports;
  status =
      napi_create_function(env, "", NAPI_AUTO_LENGTH, CreateObject, nullptr, &new_exports);
  assert(status == napi_ok);
  return new_exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
