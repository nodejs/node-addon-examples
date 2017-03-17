#include "myobject.h"
#include <assert.h>

napi_ref MyObject::constructor;

MyObject::MyObject(double value)
    : value_(value), env_(nullptr), wrapper_(nullptr) {}

MyObject::~MyObject() { napi_delete_reference(env_, wrapper_); }

void MyObject::Destructor(void* nativeObject, void* /*finalize_hint*/) {
  reinterpret_cast<MyObject*>(nativeObject)->~MyObject();
}

#define DECLARE_NAPI_METHOD(name, func)                          \
  { name, func, 0, 0, 0, napi_default, 0 }

void MyObject::Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor properties[] = {
      { "value", 0, GetValue, SetValue, 0, napi_default, 0 },
      DECLARE_NAPI_METHOD("plusOne", PlusOne),
      DECLARE_NAPI_METHOD("multiply", Multiply),
  };

  napi_value cons;
  status =
      napi_define_class(env, "MyObject", New, nullptr, 3, properties, &cons);
  assert(status == napi_ok);

  status = napi_create_reference(env, cons, 1, &constructor);
  assert(status == napi_ok);

  status = napi_set_named_property(env, exports, "MyObject", cons);
  assert(status == napi_ok);
}

void MyObject::New(napi_env env, napi_callback_info info) {
  napi_status status;

  bool is_constructor;
  status = napi_is_construct_call(env, info, &is_constructor);
  assert(status == napi_ok);

  if (is_constructor) {
    // Invoked as constructor: `new MyObject(...)`
    napi_value args[1];
    status = napi_get_cb_args(env, info, args, 1);
    assert(status == napi_ok);

    double value = 0;

    napi_valuetype valuetype;
    status = napi_get_type_of_value(env, args[0], &valuetype);
    assert(status == napi_ok);

    if (valuetype != napi_undefined) {
      status = napi_get_value_double(env, args[0], &value);
      assert(status == napi_ok);
    }

    MyObject* obj = new MyObject(value);

    napi_value jsthis;
    status = napi_get_cb_this(env, info, &jsthis);
    assert(status == napi_ok);

    obj->env_ = env;
    status = napi_wrap(env,
                       jsthis,
                       reinterpret_cast<void*>(obj),
                       MyObject::Destructor,
                       nullptr,  // finalize_hint
                       &obj->wrapper_);
    assert(status == napi_ok);

    status = napi_set_return_value(env, info, jsthis);
    assert(status == napi_ok);
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    napi_value args[1];
    status = napi_get_cb_args(env, info, args, 1);
    assert(status == napi_ok);

    const int argc = 1;
    napi_value argv[argc] = {args[0]};

    napi_value cons;
    status = napi_get_reference_value(env, constructor, &cons);
    assert(status == napi_ok);

    napi_value instance;
    status = napi_new_instance(env, cons, argc, argv, &instance);
    assert(status == napi_ok);

    status = napi_set_return_value(env, info, instance);
    assert(status == napi_ok);
  }
}

void MyObject::GetValue(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  status = napi_get_cb_this(env, info, &jsthis);
  assert(status == napi_ok);

  MyObject* obj;
  status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj));
  assert(status == napi_ok);

  napi_value num;
  status = napi_create_number(env, obj->value_, &num);
  assert(status == napi_ok);

  status = napi_set_return_value(env, info, num);
  assert(status == napi_ok);
}

void MyObject::SetValue(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value value;
  status = napi_get_cb_args(env, info, &value, 1);
  assert(status == napi_ok);

  napi_value jsthis;
  status = napi_get_cb_this(env, info, &jsthis);
  assert(status == napi_ok);

  MyObject* obj;
  status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj));
  assert(status == napi_ok);

  status = napi_get_value_double(env, value, &obj->value_);
  assert(status == napi_ok);
}

void MyObject::PlusOne(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value jsthis;
  status = napi_get_cb_this(env, info, &jsthis);
  assert(status == napi_ok);

  MyObject* obj;
  status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj));
  assert(status == napi_ok);

  obj->value_ += 1;

  napi_value num;
  status = napi_create_number(env, obj->value_, &num);
  assert(status == napi_ok);

  status = napi_set_return_value(env, info, num);
  assert(status == napi_ok);
}

void MyObject::Multiply(napi_env env, napi_callback_info info) {
  napi_status status;

  napi_value args[1];
  status = napi_get_cb_args(env, info, args, 1);
  assert(status == napi_ok);

  napi_valuetype valuetype;
  status = napi_get_type_of_value(env, args[0], &valuetype);
  assert(status == napi_ok);

  double multiple = 1;
  if (valuetype != napi_undefined) {
    status = napi_get_value_double(env, args[0], &multiple);
    assert(status == napi_ok);
  }

  napi_value jsthis;
  status = napi_get_cb_this(env, info, &jsthis);
  assert(status == napi_ok);

  MyObject* obj;
  status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj));
  assert(status == napi_ok);

  napi_value cons;
  status = napi_get_reference_value(env, constructor, &cons);
  assert(status == napi_ok);

  const int kArgCount = 1;
  napi_value argv[kArgCount];
  status = napi_create_number(env, obj->value_ * multiple, argv);
  assert(status == napi_ok);

  napi_value instance;
  status = napi_new_instance(env, cons, kArgCount, argv, &instance);
  assert(status == napi_ok);

  status = napi_set_return_value(env, info, instance);
  assert(status == napi_ok);
}
