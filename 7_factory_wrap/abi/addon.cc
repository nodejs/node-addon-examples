#include "myobject.h"

void CreateObject(node::js::value env, node::js::FunctionCallbackInfo info) {
  node::js::value args[1];
  node::js::GetCallbackArgs(info, args, 1);
  node::js::SetReturnValue(env, info, MyObject::NewInstance(env, args[0]));
}

void new_init(node::js::env env, node::js::value exports, node::js::value module) {
  MyObject::Init(env);
  node::js::SetProperty(env, module,
                        node::js::PropertyName(env, "exports"),
                        node::js::CreateFunction(env, CreateObject));
}

// NODE_MODULE's init callback's signature uses v8 type for its parameter
// which complicates our effort to create a VM neutral and ABI stable API.
// For not I am working around the issue by assuming v8 and thunking into
// an init callback with a VM neutral signature.
void InitAll(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  node::js::legacy::WorkaroundNewModuleInit(exports, module, new_init);
}

NODE_MODULE(addon, InitAll)
