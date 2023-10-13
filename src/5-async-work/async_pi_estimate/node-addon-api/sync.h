#ifndef EXAMPLES_ASYNC_PI_ESTIMATE_SYNC_H_
#define EXAMPLES_ASYNC_PI_ESTIMATE_SYNC_H_

#include <napi.h>

Napi::Value CalculateSync(const Napi::CallbackInfo& info);

#endif  // EXAMPLES_ASYNC_PI_ESTIMATE_SYNC_H_
