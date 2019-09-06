#include <napi.h>
#include "native-emitter.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  NativeEmitter::Init(env, exports);
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
