#include "myobject.h"
#include <assert.h>

MyObject::MyObject(double value)
    : value_(value), env_(nullptr), wrapper_(nullptr) {}

MyObject::~MyObject() {
  napi_delete_reference(env_, wrapper_);
}

void MyObject::Destructor(napi_env env,
                          void* nativeObject,
                          void* /*finalize_hint*/) {
  reinterpret_cast<MyObject*>(nativeObject)->~MyObject();
}

#define DECLARE_NAPI_METHOD(name, func)                                        \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value MyObject::Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor properties[] = {
      {"value", 0, 0, GetValue, SetValue, 0, napi_default, 0},
      DECLARE_NAPI_METHOD("getValue", GetValue),
      DECLARE_NAPI_METHOD("plusOne", PlusOne),
      DECLARE_NAPI_METHOD("multiply", Multiply),
  };

  napi_value cons;
  status = napi_define_class(
      env, "MyObject", NAPI_AUTO_LENGTH, New, nullptr, 4, properties, &cons);
  assert(status == napi_ok);

  // We will need the constructor `cons` later during the life cycle of the
  // addon, so we store a persistent reference to it as the instance data for
  // our addon. This will enable us to use `napi_get_instance_data` at any
  // point during the life cycle of our addon to retrieve it. We cannot simply
  // store it as a global static variable, because that will render our addon
  // unable to support Node.js worker threads and multiple contexts on a single
  // thread.
  //
  // The finalizer we pass as a lambda will be called when our addon is unloaded
  // and is responsible for releasing the persistent reference and freeing the
  // heap memory where we stored the persistent reference.
  napi_ref* constructor = new napi_ref;
  status = napi_create_reference(env, cons, 1, constructor);
  assert(status == napi_ok);
  status = napi_set_instance_data(
      env,
      constructor,
      [](napi_env env, void* data, void* hint) {
        napi_ref* constructor = static_cast<napi_ref*>(data);
        napi_status status = napi_delete_reference(env, *constructor);
        assert(status == napi_ok);
        delete constructor;
      },
      nullptr);
  assert(status == napi_ok);

  status = napi_set_named_property(env, exports, "MyObject", cons);
  assert(status == napi_ok);
  return exports;
}

napi_value MyObject::Constructor(napi_env env) {
  void* instance_data = nullptr;
  napi_status status = napi_get_instance_data(env, &instance_data);
  assert(status == napi_ok);
  napi_ref* constructor = static_cast<napi_ref*>(instance_data);

  napi_value cons;
  status = napi_get_reference_value(env, *constructor, &cons);
  assert(status == napi_ok);
  return cons;
}

napi_value MyObject::New(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value target;
  status = napi_get_new_target(env, info, &target);
  assert(status == napi_ok);
  bool is_constructor = target != nullptr;

  if (is_constructor) {
    // Invoked as constructor: `new MyObject(...)`
    size_t argc = 1;
    napi_value args[1];
    napi_value jsthis;
    status = napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr);
    assert(status == napi_ok);

    double value = 0;

    napi_valuetype valuetype;
    status = napi_typeof(env, args[0], &valuetype);
    assert(status == napi_ok);

    if (valuetype != napi_undefined) {
      status = napi_get_value_double(env, args[0], &value);
      assert(status == napi_ok);
    }

    MyObject* obj = new MyObject(value);

    obj->env_ = env;
    status = napi_wrap(env,
                       jsthis,
                       reinterpret_cast<void*>(obj),
                       MyObject::Destructor,
                       nullptr,  // finalize_hint
                       &obj->wrapper_);
    assert(status == napi_ok);

    return jsthis;
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    size_t argc_ = 1;
    napi_value args[1];
    status = napi_get_cb_info(env, info, &argc_, args, nullptr, nullptr);
    assert(status == napi_ok);

    const size_t argc = 1;
    napi_value argv[argc] = {args[0]};

    napi_value instance;
    status = napi_new_instance(env, Constructor(env), argc, argv, &instance);
    assert(status == napi_ok);

    return instance;
  }
}

napi_value MyObject::GetValue(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  status = napi_get_cb_info(env, info, nullptr, nullptr, &jsthis, nullptr);
  assert(status == napi_ok);

  MyObject* obj;
  status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj));
  assert(status == napi_ok);

  napi_value num;
  status = napi_create_double(env, obj->value_, &num);
  assert(status == napi_ok);

  return num;
}

napi_value MyObject::SetValue(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value value;
  napi_value jsthis;
  status = napi_get_cb_info(env, info, &argc, &value, &jsthis, nullptr);
  assert(status == napi_ok);

  MyObject* obj;
  status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj));
  assert(status == napi_ok);

  status = napi_get_value_double(env, value, &obj->value_);
  assert(status == napi_ok);

  return nullptr;
}

napi_value MyObject::PlusOne(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  status = napi_get_cb_info(env, info, nullptr, nullptr, &jsthis, nullptr);
  assert(status == napi_ok);

  MyObject* obj;
  status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj));
  assert(status == napi_ok);

  obj->value_ += 1;

  napi_value num;
  status = napi_create_double(env, obj->value_, &num);
  assert(status == napi_ok);

  return num;
}

napi_value MyObject::Multiply(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 1;
  napi_value args[1];
  napi_value jsthis;
  status = napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr);
  assert(status == napi_ok);

  napi_valuetype valuetype;
  status = napi_typeof(env, args[0], &valuetype);
  assert(status == napi_ok);

  double multiple = 1;
  if (valuetype != napi_undefined) {
    status = napi_get_value_double(env, args[0], &multiple);
    assert(status == napi_ok);
  }

  MyObject* obj;
  status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj));
  assert(status == napi_ok);

  const int kArgCount = 1;
  napi_value argv[kArgCount];
  status = napi_create_double(env, obj->value_ * multiple, argv);
  assert(status == napi_ok);

  napi_value instance;
  status = napi_new_instance(env, Constructor(env), kArgCount, argv, &instance);
  assert(status == napi_ok);

  return instance;
}
