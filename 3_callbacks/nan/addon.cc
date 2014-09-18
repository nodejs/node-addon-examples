#include <nan.h>

using namespace v8;

NAN_METHOD(RunCallback) {
  NanScope();

  Local<Function> cb = args[0].As<Function>();
  const unsigned argc = 1;
  Local<Value> argv[argc] = { NanNew("hello world") };
  NanMakeCallback(NanGetCurrentContext()->Global(), cb, argc, argv);
}

void Init(Handle<Object> exports, Handle<Object> module) {
  NODE_SET_METHOD(module, "exports", RunCallback);
}

NODE_MODULE(addon, Init)
