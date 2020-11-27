#ifndef EXAMPLES_ASYNC_PI_ESTIMATE_ASYNC_H_
#define EXAMPLES_ASYNC_PI_ESTIMATE_ASYNC_H_

#include <napi.h>

Napi::Value CalculateAsync(const Napi::CallbackInfo& info);

#endif  // EXAMPLES_ASYNC_PI_ESTIMATE_ASYNC_H_
