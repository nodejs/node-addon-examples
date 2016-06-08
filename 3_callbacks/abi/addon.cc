#include <node_jsvmapi.h>

void RunCallback(node::js::env env, const node::js::FunctionCallbackInfo info) {
  node::js::value args[1];
  node::js::GetCallbackArgs(info, args, 1);
  node::js::value cb = args[0];

  node::js::value argv[1];
  argv[0] = node::js::CreateString(env, "hello world");
  node::js::Call(env, node::js::GetGlobalScope(env) , cb, 1, argv);
}

void new_init(node::js::env env, node::js::value exports, node::js::value module) {
  node::js::SetProperty(env, module,
                        node::js::PropertyName(env, "exports"),
                        node::js::CreateFunction(env, RunCallback));
}

// NODE_MODULE's init callback's signature uses v8 type for its parameter
// // which complicates our effort to create a VM neutral and ABI stable API.
// // For not I am working around the issue by assuming v8 and thunking into
// // an init callback with a VM neutral signature.
void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  node::js::WorkaroundNewModuleInit(exports, module, new_init);
}

NODE_MODULE(addon, Init)
