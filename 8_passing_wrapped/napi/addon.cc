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

  return instance;
}

napi_value Add(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 2;
  napi_value args[2];
  status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
  assert(status == napi_ok);

  MyObject* obj1;
  status = napi_unwrap(env, args[0], reinterpret_cast<void**>(&obj1));
  assert(status == napi_ok);

  MyObject* obj2;
  status = napi_unwrap(env, args[1], reinterpret_cast<void**>(&obj2));
  assert(status == napi_ok);

  napi_value sum;
  status = napi_create_double(env, obj1->Val() + obj2->Val(), &sum);
  assert(status == napi_ok);

  return sum;
}

#define DECLARE_NAPI_METHOD(name, func)                          \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;

  MyObject::Init(env);

  napi_property_descriptor desc[] = {
      DECLARE_NAPI_METHOD("createObject", CreateObject),
      DECLARE_NAPI_METHOD("add", Add),
  };
  status =
      napi_define_properties(env, exports, sizeof(desc) / sizeof(*desc), desc);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
