#include <cstring>
#include <memory>
#include <string>
#include "proxy-template.h"

struct InstanceData {
  RefHolder constructor_;
  std::unique_ptr<ProxyTemplate> proxyTemplate_;
};

class NamedInterceptor {
  char buf[256];

 public:
  NamedInterceptor() { std::strncpy(this->buf, "foo", sizeof(this->buf)); }
  static napi_value Init(napi_env env, napi_value exports);
  static napi_value Constructor(napi_env env);
  static napi_value NewInstance(napi_env env);
  static napi_value New(napi_env env, napi_callback_info info);
  static napi_value CreateNew(napi_env env, napi_callback_info info);

  static napi_status ToUtf8String(napi_env env,
                                  napi_value value,
                                  std::string* result);
  static napi_status PropertyGetter(napi_env env,
                                    napi_value target,
                                    napi_value key,
                                    napi_value receiver,
                                    napi_value* result);
  static napi_status PropertySetter(napi_env env,
                                    napi_value target,
                                    napi_value key,
                                    napi_value value,
                                    napi_value receiver,
                                    bool* result);
  static napi_status PropertyDeleter(napi_env env,
                                     napi_value target,
                                     napi_value key,
                                     bool* result);
  static napi_status PropertyQuery(napi_env env,
                                   napi_value target,
                                   napi_value key,
                                   bool* result);
  static napi_status GetOwnPropertyDescriptor(napi_env env,
                                              napi_value target,
                                              napi_value key,
                                              napi_value* result);
  static napi_status OwnKeys(napi_env env,
                             napi_value target,
                             napi_value* result);
};

napi_value NamedInterceptor::CreateNew(napi_env env, napi_callback_info info) {
  return NamedInterceptor::NewInstance(env);
}

napi_value NamedInterceptor::Init(napi_env env, napi_value exports) {
  napi_value constructor{};
  NAPI_CALL(napi_define_class(env,
                              "NamedInterceptor",
                              NAPI_AUTO_LENGTH,
                              NamedInterceptor::New,
                              nullptr,
                              0,
                              nullptr,
                              &constructor));

  ProxyHandlerBuilder handlerBuilder{};
  NAPI_CALL(handlerBuilder.Get(env, &PropertyGetter));
  NAPI_CALL(handlerBuilder.Set(env, &PropertySetter));
  NAPI_CALL(handlerBuilder.DeleteProperty(env, &PropertyDeleter));
  NAPI_CALL(handlerBuilder.Has(env, &PropertyQuery));
  NAPI_CALL(
      handlerBuilder.GetOwnPropertyDescriptor(env, &GetOwnPropertyDescriptor));
  NAPI_CALL(handlerBuilder.OwnKeys(env, &OwnKeys));
  napi_value proxyHandler{};
  NAPI_CALL(handlerBuilder.NewHandler(env, &proxyHandler));
  NAPI_CALL(napi_set_instance_data(
      env,
      new InstanceData{RefHolder(env, constructor),
                       std::make_unique<ProxyTemplate>(env, proxyHandler)},
      [](napi_env /*env*/, void* data, void* /*hint*/) {
        delete static_cast<InstanceData*>(data);
      },
      nullptr));

  napi_value createFunc{};
  NAPI_CALL(napi_create_function(
      env, "create", NAPI_AUTO_LENGTH, CreateNew, nullptr, &createFunc));

  NAPI_CALL(napi_set_named_property(env, exports, "create", createFunc));
  return exports;
}

napi_value NamedInterceptor::Constructor(napi_env env) {
  InstanceData* instanceData{};
  NAPI_CALL(
      napi_get_instance_data(env, reinterpret_cast<void**>(&instanceData)));
  return instanceData->constructor_;
}

napi_value NamedInterceptor::NewInstance(napi_env env) {
  napi_escapable_handle_scope scope{};
  NAPI_CALL(napi_open_escapable_handle_scope(env, &scope));

  napi_value instance{};
  NAPI_CALL(napi_new_instance(env, Constructor(env), 0, nullptr, &instance));

  napi_value result{};
  NAPI_CALL(napi_escape_handle(env, scope, instance, &result));
  NAPI_CALL(napi_close_escapable_handle_scope(env, scope));
  return result;
}

napi_value NamedInterceptor::New(napi_env env, napi_callback_info info) {
  napi_value newTarget{};
  NAPI_CALL(napi_get_new_target(env, info, &newTarget));
  NODE_API_ASSERT_BASE(
      env, newTarget != nullptr, "Must be invoked as a constructor.", nullptr);

  napi_value proxyTarget{}, result{};
  NAPI_CALL(napi_create_function(
      env,
      "target",
      NAPI_AUTO_LENGTH,
      [](napi_env, napi_callback_info) -> napi_value { return nullptr; },
      nullptr,
      &proxyTarget));

  NamedInterceptor* obj = new NamedInterceptor();

  NAPI_CALL(napi_wrap(
      env,
      proxyTarget,
      obj,
      [](napi_env /*env*/, void* data, void* /*hint*/) {
        delete reinterpret_cast<NamedInterceptor*>(data);
      },
      nullptr,
      nullptr));

  InstanceData* instanceData{};
  NAPI_CALL(
      napi_get_instance_data(env, reinterpret_cast<void**>(&instanceData)));
  NAPI_CALL(
      instanceData->proxyTemplate_->NewInstance(env, proxyTarget, &result));
  return result;
}

napi_status NamedInterceptor::ToUtf8String(napi_env env,
                                           napi_value value,
                                           std::string* result) {
  size_t size{};
  CHECK_NAPI(napi_get_value_string_utf8(env, value, nullptr, 0, &size));
  result->assign(size, ' ');
  return napi_get_value_string_utf8(
      env, value, &(*result)[0], size + 1, nullptr);
}

napi_status NamedInterceptor::PropertyGetter(napi_env env,
                                             napi_value target,
                                             napi_value key,
                                             napi_value receiver,
                                             napi_value* result) {
  NamedInterceptor* interceptor{};
  CHECK_NAPI(napi_unwrap(env, target, reinterpret_cast<void**>(&interceptor)));
  std::string keyStr;
  CHECK_NAPI(ToUtf8String(env, key, &keyStr));
  if (keyStr == "prop") {
    return napi_create_string_utf8(
        env, interceptor->buf, NAPI_AUTO_LENGTH, result);
  } else {
    return napi_create_string_utf8(env, "bar", NAPI_AUTO_LENGTH, result);
  }
}

napi_status NamedInterceptor::PropertySetter(napi_env env,
                                             napi_value target,
                                             napi_value key,
                                             napi_value value,
                                             napi_value receiver,
                                             bool* result) {
  NamedInterceptor* interceptor{};
  CHECK_NAPI(napi_unwrap(env, target, reinterpret_cast<void**>(&interceptor)));
  std::string keyStr;
  CHECK_NAPI(ToUtf8String(env, key, &keyStr));
  if (keyStr == "prop") {
    std::string valueStr;
    CHECK_NAPI(ToUtf8String(env, value, &valueStr));
    std::strncpy(interceptor->buf, valueStr.data(), sizeof(interceptor->buf));
    *result = true;
  } else {
    *result = false;
  }
  return napi_ok;
}

napi_status NamedInterceptor::PropertyDeleter(napi_env env,
                                              napi_value target,
                                              napi_value key,
                                              bool* result) {
  NamedInterceptor* interceptor{};
  CHECK_NAPI(napi_unwrap(env, target, reinterpret_cast<void**>(&interceptor)));
  std::strncpy(interceptor->buf, "goober", sizeof(interceptor->buf));
  *result = true;
  return napi_ok;
}

napi_status NamedInterceptor::PropertyQuery(napi_env env,
                                            napi_value target,
                                            napi_value key,
                                            bool* result) {
  std::string keyStr;
  CHECK_NAPI(ToUtf8String(env, key, &keyStr));

  if (keyStr == "thing") {
    *result = true;
  }
  if (keyStr == "value") {
    *result = true;
  }
  return napi_ok;
}

napi_status NamedInterceptor::GetOwnPropertyDescriptor(napi_env env,
                                                       napi_value target,
                                                       napi_value key,
                                                       napi_value* result) {
  std::string keyStr;
  CHECK_NAPI(ToUtf8String(env, key, &keyStr));

  if (keyStr == "thing") {
    CHECK_NAPI(napi_create_object(env, result));
    napi_value trueValue{};
    CHECK_NAPI(napi_get_boolean(env, true, &trueValue));
    CHECK_NAPI(
        napi_set_named_property(env, *result, "configurable", trueValue));
    CHECK_NAPI(napi_set_named_property(env, *result, "writable", trueValue));
  }
  if (keyStr == "value") {
    CHECK_NAPI(napi_create_object(env, result));
    napi_value trueValue{};
    CHECK_NAPI(napi_get_boolean(env, true, &trueValue));
    CHECK_NAPI(
        napi_set_named_property(env, *result, "configurable", trueValue));
    CHECK_NAPI(napi_set_named_property(env, *result, "writable", trueValue));
    CHECK_NAPI(napi_set_named_property(env, *result, "enumerable", trueValue));
  }
  if (keyStr == "arguments" || keyStr == "caller") {
    CHECK_NAPI(napi_create_object(env, result));
    napi_value trueValue{}, falseValue{};
    CHECK_NAPI(napi_get_boolean(env, false, &falseValue));
    CHECK_NAPI(napi_get_boolean(env, false, &trueValue));
    CHECK_NAPI(
        napi_set_named_property(env, *result, "configurable", falseValue));
    CHECK_NAPI(napi_set_named_property(env, *result, "writable", falseValue));
    CHECK_NAPI(napi_set_named_property(env, *result, "enumerable", falseValue));
    napi_value nullValue{};
    CHECK_NAPI(napi_get_null(env, &nullValue));
    CHECK_NAPI(napi_set_named_property(env, *result, "value", nullValue));
  }
  if (keyStr == "prototype") {
    CHECK_NAPI(napi_create_object(env, result));
    napi_value trueValue{}, falseValue{};
    CHECK_NAPI(napi_get_boolean(env, false, &falseValue));
    CHECK_NAPI(napi_get_boolean(env, true, &trueValue));
    CHECK_NAPI(
        napi_set_named_property(env, *result, "configurable", falseValue));
    CHECK_NAPI(napi_set_named_property(env, *result, "writable", trueValue));
    CHECK_NAPI(napi_set_named_property(env, *result, "enumerable", falseValue));
    napi_value nullValue{};
    CHECK_NAPI(napi_get_null(env, &nullValue));
    CHECK_NAPI(napi_set_named_property(env, *result, "value", nullValue));
  }
  return napi_ok;
}

napi_status NamedInterceptor::OwnKeys(napi_env env,
                                      napi_value target,
                                      napi_value* result) {
  CHECK_NAPI(napi_get_all_property_names(env,
                                         target,
                                         napi_key_own_only,
                                         napi_key_all_properties,
                                         napi_key_keep_numbers,
                                         result));

  uint32_t arraySize{};
  CHECK_NAPI(napi_get_array_length(env, *result, &arraySize));
  napi_value firstElement{};
  CHECK_NAPI(napi_get_element(env, *result, 0, &firstElement));
  CHECK_NAPI(napi_set_element(env, *result, arraySize, firstElement));
  napi_value arraySizeValue{};
  CHECK_NAPI(napi_create_uint32(env, arraySize, &arraySizeValue));
  CHECK_NAPI(napi_set_named_property(env, *result, "length", arraySizeValue));
  napi_value value{};
  CHECK_NAPI(napi_create_string_utf8(env, "value", NAPI_AUTO_LENGTH, &value));
  CHECK_NAPI(napi_set_element(env, *result, 0, value));
  return napi_ok;
}

extern "C" napi_value Init(napi_env env, napi_value exports) {
  return NamedInterceptor::Init(env, exports);
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init);
