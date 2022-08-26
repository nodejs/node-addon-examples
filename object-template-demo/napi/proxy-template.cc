#include "proxy-template.h"
#include <vector>

namespace {

static napi_status SetTrap(napi_env env,
                           napi_value handler,
                           const char* propertyName,
                           napi_value trap) noexcept {
  if (trap != nullptr) {
    CHECK_NAPI(napi_set_named_property(env, handler, propertyName, trap));
  }
  return napi_ok;
}

using TrapCaller = napi_status (*)(napi_env env,
                                   void* trap,
                                   napi_span<napi_value> args,
                                   napi_value* result);

struct TrapCallInfo {
  TrapCaller Caller;
  void* trap;
};

template <size_t argCount>
napi_callback CreateTrapCallback() noexcept {
  return [](napi_env env, napi_callback_info info) noexcept -> napi_value {
    TrapCallInfo* trapCallInfo{};
    napi_value args[argCount]{};
    size_t actualArgCount{argCount};
    NAPI_CALL(napi_get_cb_info(env,
                               info,
                               &actualArgCount,
                               args,
                               nullptr,
                               reinterpret_cast<void**>(&trapCallInfo)));
    NODE_API_ASSERT_BASE(env,
                         actualArgCount == argCount,
                         "proxy trap requires argCount arguments.",
                         nullptr);
    napi_value result{};
    NAPI_CALL(trapCallInfo->Caller(
        env, trapCallInfo->trap, span<napi_value>(args, argCount), &result));
    return result;
  };
}

napi_status ToVector(napi_env env,
                     napi_value arrayValue,
                     std::vector<napi_value>* result) {
  bool isArray{};
  CHECK_NAPI(napi_is_array(env, arrayValue, &isArray));
  if (isArray) {
    uint32_t arraySize{};
    CHECK_NAPI(napi_get_array_length(env, arrayValue, &arraySize));
    result->reserve(result->size() + arraySize);
    for (uint32_t i = 0; i < arraySize; ++i) {
      napi_value arrayElement{};
      CHECK_NAPI(napi_get_element(env, arrayValue, i, &arrayElement));
      result->push_back(arrayElement);
    }
  }
  return napi_ok;
}

}  // namespace

napi_status ProxyHandlerBuilder::NewHandler(napi_env env,
                                            napi_value* result) noexcept {
  CHECK_NAPI(napi_create_object(env, result));

  CHECK_NAPI(SetTrap(env, *result, "apply", apply_));
  CHECK_NAPI(SetTrap(env, *result, "construct", construct_));
  CHECK_NAPI(SetTrap(env, *result, "defineProperty", defineProperty_));
  CHECK_NAPI(SetTrap(env, *result, "deleteProperty", deleteProperty_));
  CHECK_NAPI(SetTrap(env, *result, "get", get_));
  CHECK_NAPI(SetTrap(
      env, *result, "getOwnPropertyDescriptor", getOwnPropertyDescriptor_));
  CHECK_NAPI(SetTrap(env, *result, "getPrototypeOf", getPrototypeOf_));
  CHECK_NAPI(SetTrap(env, *result, "has", has_));
  CHECK_NAPI(SetTrap(env, *result, "isExtensible", isExtensible_));
  CHECK_NAPI(SetTrap(env, *result, "ownKeys", ownKeys_));
  CHECK_NAPI(SetTrap(env, *result, "preventExtensions", preventExtensions_));
  CHECK_NAPI(SetTrap(env, *result, "set", set_));
  CHECK_NAPI(SetTrap(env, *result, "setPrototypeOf", setPrototypeOf_));
  return napi_ok;
}

napi_value ProxyHandlerBuilder::Apply() noexcept {
  return apply_;
}

void ProxyHandlerBuilder::Apply(napi_value trap) noexcept {
  apply_ = trap;
}

napi_status ProxyHandlerBuilder::Apply(napi_env env,
                                       napi_callback trap,
                                       void* data) noexcept {
  return napi_create_function(
      env, "apply", NAPI_AUTO_LENGTH, trap, data, &apply_);
}

napi_status ProxyHandlerBuilder::Apply(
    napi_env env,
    napi_status (*trap)(napi_env env,
                        napi_value target,
                        napi_value thisArg,
                        napi_value argumentList,
                        napi_value* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{[](napi_env env,
                                  void* trap,
                                  napi_span<napi_value> args,
                                  napi_value* result) noexcept {
                                 return reinterpret_cast<TrapType>(trap)(
                                     env, args[0], args[1], args[2], result);
                               },
                               trap};
  return Apply(env, CreateTrapCallback<3>(), &callInfo);
}

napi_status ProxyHandlerBuilder::Apply(
    napi_env env,
    napi_status (*trap)(napi_env env,
                        napi_value target,
                        napi_value thisArg,
                        napi_span<napi_value> argumentList,
                        napi_value* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{
      [](napi_env env,
         void* trap,
         napi_span<napi_value> args,
         napi_value* result) noexcept {
        std::vector<napi_value> argVector;
        CHECK_NAPI(ToVector(env, args[2], &argVector));
        return reinterpret_cast<TrapType>(trap)(
            env,
            args[0],
            args[1],
            span<napi_value>(argVector.data(), argVector.size()),
            result);
      },
      trap};
  return Apply(env, CreateTrapCallback<3>(), &callInfo);
}

napi_value ProxyHandlerBuilder::Construct() noexcept {
  return construct_;
}

void ProxyHandlerBuilder::Construct(napi_value trap) noexcept {
  construct_ = trap;
}

napi_status ProxyHandlerBuilder::Construct(napi_env env,
                                           napi_callback trap,
                                           void* data) noexcept {
  return napi_create_function(
      env, "construct", NAPI_AUTO_LENGTH, trap, data, &construct_);
}

napi_status ProxyHandlerBuilder::Construct(
    napi_env env,
    napi_status (*trap)(napi_env env,
                        napi_value target,
                        napi_value argumentList,
                        napi_value newTarget,
                        napi_value* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{[](napi_env env,
                                  void* trap,
                                  napi_span<napi_value> args,
                                  napi_value* result) noexcept {
                                 return reinterpret_cast<TrapType>(trap)(
                                     env, args[0], args[1], args[2], result);
                               },
                               trap};
  return Construct(env, CreateTrapCallback<3>(), &callInfo);
}

napi_status ProxyHandlerBuilder::Construct(
    napi_env env,
    napi_status (*trap)(napi_env env,
                        napi_value target,
                        napi_span<napi_value> argumentList,
                        napi_value newTarget,
                        napi_value* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{
      [](napi_env env,
         void* trap,
         napi_span<napi_value> args,
         napi_value* result) noexcept {
        std::vector<napi_value> argVector;
        CHECK_NAPI(ToVector(env, args[1], &argVector));
        return reinterpret_cast<TrapType>(trap)(
            env,
            args[0],
            span<napi_value>(argVector.data(), argVector.size()),
            args[2],
            result);
      },
      trap};
  return Construct(env, CreateTrapCallback<3>(), &callInfo);
}

napi_value ProxyHandlerBuilder::DefineProperty() noexcept {
  return defineProperty_;
}

void ProxyHandlerBuilder::DefineProperty(napi_value trap) noexcept {
  defineProperty_ = trap;
}

napi_status ProxyHandlerBuilder::DefineProperty(napi_env env,
                                                napi_callback trap,
                                                void* data) noexcept {
  return napi_create_function(
      env, "defineProperty", NAPI_AUTO_LENGTH, trap, data, &defineProperty_);
}

napi_status ProxyHandlerBuilder::DefineProperty(
    napi_env env,
    napi_status (*trap)(napi_env env,
                        napi_value target,
                        napi_value key,
                        napi_value descriptor,
                        napi_value* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{[](napi_env env,
                                  void* trap,
                                  napi_span<napi_value> args,
                                  napi_value* result) noexcept {
                                 return reinterpret_cast<TrapType>(trap)(
                                     env, args[0], args[1], args[2], result);
                               },
                               trap};
  return DefineProperty(env, CreateTrapCallback<3>(), &callInfo);
}

napi_status ProxyHandlerBuilder::DefineProperty(
    napi_env env,
    napi_status (*trap)(napi_env env,
                        napi_value target,
                        napi_value key,
                        napi_value descriptor,
                        bool* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{
      [](napi_env env,
         void* trap,
         napi_span<napi_value> args,
         napi_value* result) noexcept {
        bool boolResult{};
        CHECK_NAPI(reinterpret_cast<TrapType>(trap)(
            env, args[0], args[1], args[2], &boolResult));
        return napi_get_boolean(env, boolResult, result);
      },
      trap};
  return DefineProperty(env, CreateTrapCallback<3>(), &callInfo);
}

napi_value ProxyHandlerBuilder::DeleteProperty() noexcept {
  return deleteProperty_;
}

void ProxyHandlerBuilder::DeleteProperty(napi_value trap) noexcept {
  deleteProperty_ = trap;
}

napi_status ProxyHandlerBuilder::DeleteProperty(napi_env env,
                                                napi_callback trap,
                                                void* data) noexcept {
  return napi_create_function(
      env, "deleteProperty", NAPI_AUTO_LENGTH, trap, data, &deleteProperty_);
}

napi_status ProxyHandlerBuilder::DeleteProperty(
    napi_env env,
    napi_status (*trap)(
        napi_env env, napi_value target, napi_value key, napi_value* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{[](napi_env env,
                                  void* trap,
                                  napi_span<napi_value> args,
                                  napi_value* result) noexcept {
                                 return reinterpret_cast<TrapType>(trap)(
                                     env, args[0], args[1], result);
                               },
                               trap};
  return DeleteProperty(env, CreateTrapCallback<2>(), &callInfo);
}

napi_status ProxyHandlerBuilder::DeleteProperty(
    napi_env env,
    napi_status (*trap)(
        napi_env env, napi_value target, napi_value key, bool* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{[](napi_env env,
                                  void* trap,
                                  napi_span<napi_value> args,
                                  napi_value* result) noexcept {
                                 bool boolResult{};
                                 CHECK_NAPI(reinterpret_cast<TrapType>(trap)(
                                     env, args[0], args[1], &boolResult));
                                 return napi_get_boolean(
                                     env, boolResult, result);
                               },
                               trap};
  return DeleteProperty(env, CreateTrapCallback<2>(), &callInfo);
}

napi_value ProxyHandlerBuilder::Get() noexcept {
  return get_;
}

void ProxyHandlerBuilder::Get(napi_value trap) noexcept {
  get_ = trap;
}

napi_status ProxyHandlerBuilder::Get(napi_env env,
                                     napi_callback trap,
                                     void* data) noexcept {
  return napi_create_function(env, "get", NAPI_AUTO_LENGTH, trap, data, &get_);
}

napi_status ProxyHandlerBuilder::Get(napi_env env,
                                     napi_status (*trap)(napi_env env,
                                                         napi_value target,
                                                         napi_value key,
                                                         napi_value receiver,
                                                         napi_value* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{[](napi_env env,
                                  void* trap,
                                  napi_span<napi_value> args,
                                  napi_value* result) noexcept {
                                 return reinterpret_cast<TrapType>(trap)(
                                     env, args[0], args[1], args[2], result);
                               },
                               trap};
  return Get(env, CreateTrapCallback<3>(), &callInfo);
}

napi_value ProxyHandlerBuilder::GetOwnPropertyDescriptor() noexcept {
  return getOwnPropertyDescriptor_;
}

void ProxyHandlerBuilder::GetOwnPropertyDescriptor(napi_value trap) noexcept {
  getOwnPropertyDescriptor_ = trap;
}

napi_status ProxyHandlerBuilder::GetOwnPropertyDescriptor(napi_env env,
                                                          napi_callback trap,
                                                          void* data) noexcept {
  return napi_create_function(env,
                              "getOwnPropertyDescriptor",
                              NAPI_AUTO_LENGTH,
                              trap,
                              data,
                              &getOwnPropertyDescriptor_);
}

napi_status ProxyHandlerBuilder::GetOwnPropertyDescriptor(
    napi_env env,
    napi_status (*trap)(
        napi_env env, napi_value target, napi_value key, napi_value* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{[](napi_env env,
                                  void* trap,
                                  napi_span<napi_value> args,
                                  napi_value* result) noexcept {
                                 return reinterpret_cast<TrapType>(trap)(
                                     env, args[0], args[1], result);
                               },
                               trap};
  return GetOwnPropertyDescriptor(env, CreateTrapCallback<2>(), &callInfo);
}

napi_value ProxyHandlerBuilder::GetPrototypeOf() noexcept {
  return getPrototypeOf_;
}

void ProxyHandlerBuilder::GetPrototypeOf(napi_value trap) noexcept {
  getPrototypeOf_ = trap;
}

napi_status ProxyHandlerBuilder::GetPrototypeOf(napi_env env,
                                                napi_callback trap,
                                                void* data) noexcept {
  return napi_create_function(
      env, "getPrototypeOf", NAPI_AUTO_LENGTH, trap, data, &getPrototypeOf_);
}

napi_status ProxyHandlerBuilder::GetPrototypeOf(
    napi_env env,
    napi_status (*trap)(napi_env env, napi_value target, napi_value* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{[](napi_env env,
                                  void* trap,
                                  napi_span<napi_value> args,
                                  napi_value* result) noexcept {
                                 return reinterpret_cast<TrapType>(trap)(
                                     env, args[0], result);
                               },
                               trap};
  return GetPrototypeOf(env, CreateTrapCallback<1>(), &callInfo);
}

napi_value ProxyHandlerBuilder::Has() noexcept {
  return has_;
}

void ProxyHandlerBuilder::Has(napi_value trap) noexcept {
  has_ = trap;
}

napi_status ProxyHandlerBuilder::Has(napi_env env,
                                     napi_callback trap,
                                     void* data) noexcept {
  return napi_create_function(env, "has", NAPI_AUTO_LENGTH, trap, data, &has_);
}

napi_status ProxyHandlerBuilder::Has(napi_env env,
                                     napi_status (*trap)(napi_env env,
                                                         napi_value target,
                                                         napi_value key,
                                                         napi_value* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{[](napi_env env,
                                  void* trap,
                                  napi_span<napi_value> args,
                                  napi_value* result) noexcept {
                                 return reinterpret_cast<TrapType>(trap)(
                                     env, args[0], args[1], result);
                               },
                               trap};
  return Has(env, CreateTrapCallback<2>(), &callInfo);
}

napi_status ProxyHandlerBuilder::Has(napi_env env,
                                     napi_status (*trap)(napi_env env,
                                                         napi_value target,
                                                         napi_value key,
                                                         bool* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{[](napi_env env,
                                  void* trap,
                                  napi_span<napi_value> args,
                                  napi_value* result) noexcept {
                                 bool boolResult{};
                                 CHECK_NAPI(reinterpret_cast<TrapType>(trap)(
                                     env, args[0], args[1], &boolResult));
                                 return napi_get_boolean(
                                     env, boolResult, result);
                               },
                               trap};
  return Has(env, CreateTrapCallback<2>(), &callInfo);
}

napi_value ProxyHandlerBuilder::IsExtensible() noexcept {
  return isExtensible_;
}

void ProxyHandlerBuilder::IsExtensible(napi_value trap) noexcept {
  isExtensible_ = trap;
}

napi_status ProxyHandlerBuilder::IsExtensible(napi_env env,
                                              napi_callback trap,
                                              void* data) noexcept {
  return napi_create_function(
      env, "isExtensible", NAPI_AUTO_LENGTH, trap, data, &isExtensible_);
}

napi_status ProxyHandlerBuilder::IsExtensible(
    napi_env env,
    napi_status (*trap)(napi_env env, napi_value target, napi_value* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{[](napi_env env,
                                  void* trap,
                                  napi_span<napi_value> args,
                                  napi_value* result) noexcept {
                                 return reinterpret_cast<TrapType>(trap)(
                                     env, args[0], result);
                               },
                               trap};
  return IsExtensible(env, CreateTrapCallback<1>(), &callInfo);
}

napi_status ProxyHandlerBuilder::IsExtensible(
    napi_env env,
    napi_status (*trap)(napi_env env, napi_value target, bool* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{
      [](napi_env env,
         void* trap,
         napi_span<napi_value> args,
         napi_value* result) noexcept {
        bool boolResult{};
        CHECK_NAPI(reinterpret_cast<TrapType>(trap)(env, args[0], &boolResult));
        return napi_get_boolean(env, boolResult, result);
      },
      trap};
  return IsExtensible(env, CreateTrapCallback<1>(), &callInfo);
}

napi_value ProxyHandlerBuilder::OwnKeys() noexcept {
  return ownKeys_;
}

void ProxyHandlerBuilder::OwnKeys(napi_value trap) noexcept {
  ownKeys_ = trap;
}

napi_status ProxyHandlerBuilder::OwnKeys(napi_env env,
                                         napi_callback trap,
                                         void* data) noexcept {
  return napi_create_function(
      env, "ownKeys", NAPI_AUTO_LENGTH, trap, data, &ownKeys_);
}

napi_status ProxyHandlerBuilder::OwnKeys(
    napi_env env,
    napi_status (*trap)(napi_env env, napi_value target, napi_value* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{[](napi_env env,
                                  void* trap,
                                  napi_span<napi_value> args,
                                  napi_value* result) noexcept {
                                 return reinterpret_cast<TrapType>(trap)(
                                     env, args[0], result);
                               },
                               trap};
  return OwnKeys(env, CreateTrapCallback<1>(), &callInfo);
}

napi_value ProxyHandlerBuilder::PreventExtensions() noexcept {
  return preventExtensions_;
}

void ProxyHandlerBuilder::PreventExtensions(napi_value trap) noexcept {
  preventExtensions_ = trap;
}

napi_status ProxyHandlerBuilder::PreventExtensions(napi_env env,
                                                   napi_callback trap,
                                                   void* data) noexcept {
  return napi_create_function(env,
                              "preventExtensions",
                              NAPI_AUTO_LENGTH,
                              trap,
                              data,
                              &preventExtensions_);
}

napi_status ProxyHandlerBuilder::PreventExtensions(
    napi_env env,
    napi_status (*trap)(napi_env env, napi_value target, napi_value* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{[](napi_env env,
                                  void* trap,
                                  napi_span<napi_value> args,
                                  napi_value* result) noexcept {
                                 return reinterpret_cast<TrapType>(trap)(
                                     env, args[0], result);
                               },
                               trap};
  return PreventExtensions(env, CreateTrapCallback<1>(), &callInfo);
}

napi_status ProxyHandlerBuilder::PreventExtensions(
    napi_env env,
    napi_status (*trap)(napi_env env, napi_value target, bool* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{
      [](napi_env env,
         void* trap,
         napi_span<napi_value> args,
         napi_value* result) noexcept {
        bool boolResult{};
        CHECK_NAPI(reinterpret_cast<TrapType>(trap)(env, args[0], &boolResult));
        return napi_get_boolean(env, boolResult, result);
      },
      trap};
  return PreventExtensions(env, CreateTrapCallback<1>(), &callInfo);
}

napi_value ProxyHandlerBuilder::Set() noexcept {
  return set_;
}

void ProxyHandlerBuilder::Set(napi_value trap) noexcept {
  set_ = trap;
}

napi_status ProxyHandlerBuilder::Set(napi_env env,
                                     napi_callback trap,
                                     void* data) noexcept {
  return napi_create_function(env, "set", NAPI_AUTO_LENGTH, trap, data, &set_);
}

napi_status ProxyHandlerBuilder::Set(napi_env env,
                                     napi_status (*trap)(napi_env env,
                                                         napi_value target,
                                                         napi_value key,
                                                         napi_value value,
                                                         napi_value receiver,
                                                         napi_value* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{
      [](napi_env env,
         void* trap,
         napi_span<napi_value> args,
         napi_value* result) noexcept {
        return reinterpret_cast<TrapType>(trap)(
            env, args[0], args[1], args[2], args[3], result);
      },
      trap};
  return Set(env, CreateTrapCallback<4>(), &callInfo);
}

napi_status ProxyHandlerBuilder::Set(napi_env env,
                                     napi_status (*trap)(napi_env env,
                                                         napi_value target,
                                                         napi_value key,
                                                         napi_value value,
                                                         napi_value receiver,
                                                         bool* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{
      [](napi_env env,
         void* trap,
         napi_span<napi_value> args,
         napi_value* result) noexcept {
        bool boolResult{};
        CHECK_NAPI(reinterpret_cast<TrapType>(trap)(
            env, args[0], args[1], args[2], args[3], &boolResult));
        return napi_get_boolean(env, boolResult, result);
      },
      trap};
  return Set(env, CreateTrapCallback<4>(), &callInfo);
}

napi_value ProxyHandlerBuilder::SetPrototypeOf() noexcept {
  return setPrototypeOf_;
}

void ProxyHandlerBuilder::SetPrototypeOf(napi_value trap) noexcept {
  setPrototypeOf_ = trap;
}

napi_status ProxyHandlerBuilder::SetPrototypeOf(napi_env env,
                                                napi_callback trap,
                                                void* data) noexcept {
  return napi_create_function(
      env, "setPrototypeOf", NAPI_AUTO_LENGTH, trap, data, &setPrototypeOf_);
}

napi_status ProxyHandlerBuilder::SetPrototypeOf(
    napi_env env,
    napi_status (*trap)(napi_env env,
                        napi_value target,
                        napi_value prototype,
                        napi_value* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{[](napi_env env,
                                  void* trap,
                                  napi_span<napi_value> args,
                                  napi_value* result) noexcept {
                                 return reinterpret_cast<TrapType>(trap)(
                                     env, args[0], args[1], result);
                               },
                               trap};
  return SetPrototypeOf(env, CreateTrapCallback<2>(), &callInfo);
}

napi_status ProxyHandlerBuilder::SetPrototypeOf(
    napi_env env,
    napi_status (*trap)(
        napi_env env, napi_value target, napi_value prototype, bool* result)) {
  using TrapType = decltype(trap);
  static TrapCallInfo callInfo{[](napi_env env,
                                  void* trap,
                                  napi_span<napi_value> args,
                                  napi_value* result) noexcept {
                                 bool boolResult{};
                                 CHECK_NAPI(reinterpret_cast<TrapType>(trap)(
                                     env, args[0], args[1], &boolResult));
                                 return napi_get_boolean(
                                     env, boolResult, result);
                               },
                               trap};
  return SetPrototypeOf(env, CreateTrapCallback<2>(), &callInfo);
}

//=============

ProxyTemplate::ProxyTemplate(napi_env env, napi_value proxyHandler) noexcept
    : proxyHandler_(RefHolder(env, proxyHandler)) {}

napi_status ProxyTemplate::NewInstance(napi_env env,
                                       napi_value target,
                                       napi_value* result) noexcept {
  napi_value proxyConstructor{};
  CHECK_NAPI(GetProxyConstructor(env, &proxyConstructor));

  napi_value args[] = {target, static_cast<napi_value>(proxyHandler_)};
  return napi_new_instance(env, proxyConstructor, 2, args, result);
}

napi_status ProxyTemplate::GetProxyConstructor(napi_env env,
                                               napi_value* result) noexcept {
  if (!proxyConstructor_) {
    napi_value global{}, proxyConstructor{};
    CHECK_NAPI(napi_get_global(env, &global));
    CHECK_NAPI(
        napi_get_named_property(env, global, "Proxy", &proxyConstructor));
    proxyConstructor_ = RefHolder(env, proxyConstructor);
  }
  *result = proxyConstructor_;
  return napi_ok;
}
