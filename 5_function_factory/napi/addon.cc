#include <node_api.h>
#include <assert.h>

void MyFunction(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value str;
  status = napi_create_string_utf8(env, "hello world", -1, &str);
  assert(status == napi_ok);

  status = napi_set_return_value(env, info, str);
  assert(status == napi_ok);
}

void CreateFunction(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value fn;
  status = napi_create_function(env, "theFunction", MyFunction, nullptr, &fn);
  assert(status == napi_ok);

  status = napi_set_return_value(env, info, fn);
  assert(status == napi_ok);
}

#define DECLARE_NAPI_METHOD(name, func)                          \
  { name, func, 0, 0, 0, napi_default, 0 }

void Init(napi_env env, napi_value exports, napi_value module, void* priv) {
  napi_status status;
  napi_property_descriptor desc =
      DECLARE_NAPI_METHOD("exports", CreateFunction);
  status = napi_define_properties(env, module, 1, &desc);
  assert(status == napi_ok);
}

NAPI_MODULE(addon, Init)

