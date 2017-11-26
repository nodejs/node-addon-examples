#include <node_api.h>
#include <assert.h>

napi_value MyFunction(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value str;
  status = napi_create_string_utf8(env, "hello world", NAPI_AUTO_LENGTH, &str);
  assert(status == napi_ok);

  return str;
}

napi_value CreateFunction(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value fn;
  status = napi_create_function(env, "theFunction", NAPI_AUTO_LENGTH, MyFunction, nullptr, &fn);
  assert(status == napi_ok);

  return fn;
}

napi_value Init(napi_env env, napi_value exports) {
  napi_value new_exports;
  napi_status status =
      napi_create_function(env, "", NAPI_AUTO_LENGTH, CreateFunction, nullptr, &new_exports);
  assert(status == napi_ok);
  return new_exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
