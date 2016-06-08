#include <node_jsvmapi.h>

void Add(node::js::env env, node::js::FunctionCallbackInfo info) {
    if (node::js::GetCallbackArgsLength(info) < 2) {
        node::js::ThrowError(
            env,
            node::js::CreateTypeError(
                env,
                node::js::CreateString(env, "Wrong number of arguments")));
        return;
    }

    node::js::value args[2];
    node::js::GetCallbackArgs(info, args, 2);

    if (node::js::GetTypeOfValue(args[0]) != node::js::valuetype::Number ||
        node::js::GetTypeOfValue(args[1]) != node::js::valuetype::Number) {
        node::js::ThrowError(
            env,
            node::js::CreateTypeError(
                env,
                node::js::CreateString(env, "Wrong arguments")));
        return;
    }

    double value = node::js::GetNumberFromValue(args[0]) + node::js::GetNumberFromValue(args[1]);
    node::js::value num = node::js::CreateNumber(env, value);

    node::js::SetReturnValue(env, info, num);
}


void new_init(node::js::env env, node::js::value exports, node::js::value module) {
  node::js::SetProperty(env, exports,
                        node::js::PropertyName(env, "add"),
                        node::js::CreateFunction(env, Add));
}

// NODE_MODULE's init callback's signature uses v8 type for its parameter
// // which complicates our effort to create a VM neutral and ABI stable API.
// // For not I am working around the issue by assuming v8 and thunking into
// // an init callback with a VM neutral signature.
void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  node::js::WorkaroundNewModuleInit(exports, module, new_init);
}

NODE_MODULE(addon, Init)
