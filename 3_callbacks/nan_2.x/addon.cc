#include <nan.h>

using namespace v8;

NAN_METHOD(RunCallback) {
  Local<Function> cb = info[0].As<Function>();
  const unsigned argc = 1;
  Local<Value> argv[argc] = { Nan::New<v8::String>("hello world").ToLocalChecked() };
  Nan::MakeCallback(info.This(), cb, argc, argv);

  info.GetReturnValue().SetUndefined();
}

void Init(Handle<Object> exports, Handle<Object> module) {
  Nan::SetMethod(module
    , "exports"
    , RunCallback
  );
}

NODE_MODULE(addon, Init)
