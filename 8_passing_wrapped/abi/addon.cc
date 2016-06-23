#include "myobject.h"

void napi_create_object(napi_env env, napi_func_cb_info info) {
  napi_value args[1];
  napi_get_cb_args(env, info, args, 1);
  napi_set_return_value(env, info, MyObject::NewInstance(env, args[0]));
}

void Add(napi_env env, napi_func_cb_info info) {
  napi_value args[2];
  napi_get_cb_args(env, info, args, 2);
  MyObject* obj1 = (MyObject*) napi_unwrap(env, args[0]);
  MyObject* obj2 = (MyObject*) napi_unwrap(env, args[1]);
  double sum = obj1->Val() + obj2->Val();
  napi_set_return_value(env, info, napi_create_number(env,sum));
}

void new_init(napi_env env, napi_value exports, napi_value module) {
  MyObject::Init(env);

  napi_set_property(env, exports,
              napi_proterty_name(env, "createObject"),
              napi_create_function(env, napi_create_object));


  napi_set_property(env, exports,
              napi_proterty_name(env, "add"),
              napi_create_function(env, Add));
}

//NODE_MODULE's init napi_callback's signature uses v8 type for its parameter
// which complicates our effort to create a VM neutral and ABI stable API.
// For not I am working around the issue by assuming v8 and thunking into
// an init napi_callback with a VM neutral signature.
void InitAll(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  WorkaroundNewModuleInit(exports, module, new_init);
}

NODE_MODULE(addon, InitAll)
