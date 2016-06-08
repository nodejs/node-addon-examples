#include <node_jsvmapi.h>

void Method(node::js::env env, node::js::FunctionCallbackInfo info) {
    node::js::SetReturnValue(
        env,
        info,
        node::js::CreateString(env, "world"));
}

void new_init(node::js::env env, node::js::value exports, node::js::value module) {
  node::js::SetProperty(env, exports,
                        node::js::PropertyName(env, "hello"),
                        node::js::CreateFunction(env, Method));
}


// NODE_MODULE's init callback's signature uses v8 type for its parameter
// // which complicates our effort to create a VM neutral and ABI stable API.
// // For not I am working around the issue by assuming v8 and thunking into
// // an init callback with a VM neutral signature.
void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  node::js::WorkaroundNewModuleInit(exports, module, new_init);
}

NODE_MODULE(addon, Init)

