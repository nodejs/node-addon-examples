#include "myobject.h"

using namespace node::js;

void CreateObject(node::js::value env, node::js::FunctionCallbackInfo info) {
  node::js::value args[1];
  node::js::GetCallbackArgs(env, info, args, 1);
  node::js::SetReturnValue(env, info, MyObject::NewInstance(env, args[0]));
}

void Add(node::js::value env, node::js::FunctionCallbackInfo info) {
  node::js::value args[2];
  node::js::GetCallbackArgs(env, info, args, 2);
  MyObject* obj1 = (MyObject*) node::js::Unwrap(env, args[0]);
  MyObject* obj2 = (MyObject*) node::js::Unwrap(env, args[1]);
  double sum = obj1->Val() + obj2->Val();
  node::js::SetReturnValue(env, info, node::js::CreateNumber(env,sum));
}

void new_init(env env, node::js::value exports, node::js::value module) {
  MyObject::Init(env);

  SetProperty(env, exports,
              node::js::PropertyName(env, "createObject"),
              node::js::CreateFunction(env, CreateObject));


  SetProperty(env, exports,
              node::js::PropertyName(env, "add"),
              node::js::CreateFunction(env, Add));
}

//NODE_MODULE's init callback's signature uses v8 type for its parameter
// which complicates our effort to create a VM neutral and ABI stable API.
// For not I am working around the issue by assuming v8 and thunking into
// an init callback with a VM neutral signature.
void InitAll(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  legacy::WorkaroundNewModuleInit(exports, module, new_init);
}

NODE_MODULE(addon, InitAll)
