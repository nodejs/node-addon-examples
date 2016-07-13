#include "myobject.h"

void Init(napi_env env, napi_value exports, napi_value module) {
  MyObject::Init(env, exports);
}

NODE_MODULE_ABI(addon, Init)
