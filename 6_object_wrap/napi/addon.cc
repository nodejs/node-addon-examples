#include "myobject.h"

void Init(napi_env env, napi_value exports, napi_value module) {
  MyObject::Init(env, exports);
}

NAPI_MODULE(addon, Init)
