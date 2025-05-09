#include "myobject-napi.h"
#include "otherobject-napi.h"

Napi::Object init(Napi::Env env, Napi::Object exports) {
  MyObjectNapi::init(env, exports);
  OtherObjectNapi::init(env, exports);
  
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, init)
