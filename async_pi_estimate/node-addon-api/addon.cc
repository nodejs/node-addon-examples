#include <napi.h>
#include "async.h"  // NOLINT(build/include)
#include "sync.h"   // NOLINT(build/include)

// Expose synchronous and asynchronous access to our
// Estimate() function
Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "calculateSync"),
              Napi::Function::New(env, CalculateSync));
  exports.Set(Napi::String::New(env, "calculateAsync"),
              Napi::Function::New(env, CalculateAsync));
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
