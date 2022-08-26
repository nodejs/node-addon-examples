#include "node-api-common.h"

template <typename T>
using napi_span = span<T>;

// Creates a handler object for a JavaScript Proxy.
// See
// https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Proxy
// for details about JavaScript Proxy.
//
// There are four different ways we can set each Proxy handler trap:
// - using napi_value function.
// - using napi_callback that creates function.
// - using a callback signature that matches Proxy handler definition arguments.
// - using a callback based on the Proxy handler trap signature where we replace
//   napi_value with strongly typed values such as bool or
//   napi_span<napi_value>.
//
// Note that ProxyHandlerBuilder keeps handler trap functions as napi_value
// and thus cannot be persisted. The typical usage is to create new
// ProxyHandlerBuilder instance on the call stack, set handler traps, create
// handler to pass to Proxy constructor or ProxyTemplate, and not to reuse the
// ProxyHandlerBuilder instance.
struct ProxyHandlerBuilder {
  napi_status NewHandler(napi_env env, napi_value* result) noexcept;

  napi_value Apply() noexcept;
  void Apply(napi_value trap) noexcept;
  napi_status Apply(napi_env env,
                    napi_callback trap,
                    void* data = nullptr) noexcept;
  napi_status Apply(napi_env env,
                    napi_status (*trap)(napi_env env,
                                        napi_value target,
                                        napi_value thisArg,
                                        napi_value argumentList,
                                        napi_value* result));
  napi_status Apply(napi_env env,
                    napi_status (*trap)(napi_env env,
                                        napi_value target,
                                        napi_value thisArg,
                                        napi_span<napi_value> argumentList,
                                        napi_value* result));

  napi_value Construct() noexcept;
  void Construct(napi_value trap) noexcept;
  napi_status Construct(napi_env env,
                        napi_callback trap,
                        void* data = nullptr) noexcept;
  napi_status Construct(napi_env env,
                        napi_status (*trap)(napi_env env,
                                            napi_value target,
                                            napi_value argumentList,
                                            napi_value newTarget,
                                            napi_value* result));
  napi_status Construct(napi_env env,
                        napi_status (*trap)(napi_env env,
                                            napi_value target,
                                            napi_span<napi_value> argumentList,
                                            napi_value newTarget,
                                            napi_value* result));

  napi_value DefineProperty() noexcept;
  void DefineProperty(napi_value trap) noexcept;
  napi_status DefineProperty(napi_env env,
                             napi_callback trap,
                             void* data = nullptr) noexcept;
  napi_status DefineProperty(napi_env env,
                             napi_status (*trap)(napi_env env,
                                                 napi_value target,
                                                 napi_value key,
                                                 napi_value descriptor,
                                                 napi_value* result));
  napi_status DefineProperty(napi_env env,
                             napi_status (*trap)(napi_env env,
                                                 napi_value target,
                                                 napi_value key,
                                                 napi_value descriptor,
                                                 bool* result));

  napi_value DeleteProperty() noexcept;
  void DeleteProperty(napi_value trap) noexcept;
  napi_status DeleteProperty(napi_env env,
                             napi_callback trap,
                             void* data = nullptr) noexcept;
  napi_status DeleteProperty(napi_env env,
                             napi_status (*trap)(napi_env env,
                                                 napi_value target,
                                                 napi_value key,
                                                 napi_value* result));
  napi_status DeleteProperty(napi_env env,
                             napi_status (*trap)(napi_env env,
                                                 napi_value target,
                                                 napi_value key,
                                                 bool* result));

  napi_value Get() noexcept;
  void Get(napi_value trap) noexcept;
  napi_status Get(napi_env env,
                  napi_callback trap,
                  void* data = nullptr) noexcept;
  napi_status Get(napi_env env,
                  napi_status (*trap)(napi_env env,
                                      napi_value target,
                                      napi_value key,
                                      napi_value receiver,
                                      napi_value* result));

  napi_value GetOwnPropertyDescriptor() noexcept;
  void GetOwnPropertyDescriptor(napi_value trap) noexcept;
  napi_status GetOwnPropertyDescriptor(napi_env env,
                                       napi_callback trap,
                                       void* data = nullptr) noexcept;
  napi_status GetOwnPropertyDescriptor(napi_env env,
                                       napi_status (*trap)(napi_env env,
                                                           napi_value target,
                                                           napi_value key,
                                                           napi_value* result));

  napi_value GetPrototypeOf() noexcept;
  void GetPrototypeOf(napi_value trap) noexcept;
  napi_status GetPrototypeOf(napi_env env,
                             napi_callback trap,
                             void* data = nullptr) noexcept;
  napi_status GetPrototypeOf(napi_env env,
                             napi_status (*trap)(napi_env env,
                                                 napi_value target,
                                                 napi_value* result));

  napi_value Has() noexcept;
  void Has(napi_value trap) noexcept;
  napi_status Has(napi_env env,
                  napi_callback trap,
                  void* data = nullptr) noexcept;
  napi_status Has(napi_env env,
                  napi_status (*trap)(napi_env env,
                                      napi_value target,
                                      napi_value key,
                                      napi_value* result));
  napi_status Has(napi_env env,
                  napi_status (*trap)(napi_env env,
                                      napi_value target,
                                      napi_value key,
                                      bool* result));

  napi_value IsExtensible() noexcept;
  void IsExtensible(napi_value trap) noexcept;
  napi_status IsExtensible(napi_env env,
                           napi_callback trap,
                           void* data = nullptr) noexcept;
  napi_status IsExtensible(napi_env env,
                           napi_status (*trap)(napi_env env,
                                               napi_value target,
                                               napi_value* result));
  napi_status IsExtensible(napi_env env,
                           napi_status (*trap)(napi_env env,
                                               napi_value target,
                                               bool* result));

  napi_value OwnKeys() noexcept;
  void OwnKeys(napi_value trap) noexcept;
  napi_status OwnKeys(napi_env env,
                      napi_callback trap,
                      void* data = nullptr) noexcept;
  napi_status OwnKeys(napi_env env,
                      napi_status (*trap)(napi_env env,
                                          napi_value target,
                                          napi_value* result));

  napi_value PreventExtensions() noexcept;
  void PreventExtensions(napi_value trap) noexcept;
  napi_status PreventExtensions(napi_env env,
                                napi_callback trap,
                                void* data = nullptr) noexcept;
  napi_status PreventExtensions(napi_env env,
                                napi_status (*trap)(napi_env env,
                                                    napi_value target,
                                                    napi_value* result));
  napi_status PreventExtensions(napi_env env,
                                napi_status (*trap)(napi_env env,
                                                    napi_value target,
                                                    bool* result));

  napi_value Set() noexcept;
  void Set(napi_value trap) noexcept;
  napi_status Set(napi_env env,
                  napi_callback trap,
                  void* data = nullptr) noexcept;
  napi_status Set(napi_env env,
                  napi_status (*trap)(napi_env env,
                                      napi_value target,
                                      napi_value key,
                                      napi_value value,
                                      napi_value receiver,
                                      napi_value* result));
  napi_status Set(napi_env env,
                  napi_status (*trap)(napi_env env,
                                      napi_value target,
                                      napi_value key,
                                      napi_value value,
                                      napi_value receiver,
                                      bool* result));

  napi_value SetPrototypeOf() noexcept;
  void SetPrototypeOf(napi_value trap) noexcept;
  napi_status SetPrototypeOf(napi_env env,
                             napi_callback trap,
                             void* data = nullptr) noexcept;
  napi_status SetPrototypeOf(napi_env env,
                             napi_status (*trap)(napi_env env,
                                                 napi_value target,
                                                 napi_value prototype,
                                                 napi_value* result));
  napi_status SetPrototypeOf(napi_env env,
                             napi_status (*trap)(napi_env env,
                                                 napi_value target,
                                                 napi_value prototype,
                                                 bool* result));

 private:
  napi_value apply_{};
  napi_value construct_{};
  napi_value defineProperty_{};
  napi_value deleteProperty_{};
  napi_value get_{};
  napi_value getOwnPropertyDescriptor_{};
  napi_value getPrototypeOf_{};
  napi_value has_{};
  napi_value isExtensible_{};
  napi_value ownKeys_{};
  napi_value preventExtensions_{};
  napi_value set_{};
  napi_value setPrototypeOf_{};
};

struct ProxyTemplate {
  ProxyTemplate(napi_env env, napi_value proxyHandler) noexcept;

  napi_status NewInstance(napi_env env,
                          napi_value target,
                          napi_value* result) noexcept;

 private:
  napi_status GetProxyConstructor(napi_env env, napi_value* result) noexcept;

 private:
  RefHolder proxyConstructor_{};
  RefHolder proxyHandler_{};
};
