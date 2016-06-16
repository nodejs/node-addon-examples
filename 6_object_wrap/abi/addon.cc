#include "myobject.h"

void new_init(node::js::env env, node::js::value exports, node::js::value module) {
  MyObject::Init(env, exports);
}

// NODE_MODULE's init callback's signature uses v8 type for its parameter
// // which complicates our effort to create a VM neutral and ABI stable API.
// // For not I am working around the issue by assuming v8 and thunking into
// // an init callback with a VM neutral signature.
void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  node::js::legacy::WorkaroundNewModuleInit(exports, module, new_init);
}

NODE_MODULE(addon, Init)
