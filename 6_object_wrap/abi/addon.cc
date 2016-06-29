#include "myobject.h"

void Init(napi_env napi_env, napi_value exports, napi_value module) {
  MyObject::Init(napi_env, exports);
}

NODE_MODULE_ABI(addon, Init)
