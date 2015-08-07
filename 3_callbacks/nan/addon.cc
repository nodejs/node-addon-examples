#include <nan.h>

NAN_METHOD(RunCallback) {
  v8::Local<v8::Function> cb = info[0].As<v8::Function>();
  const unsigned argc = 1;
  v8::Local<v8::Value> argv[argc] = { Nan::New<v8::String>("hello world").ToLocalChecked() };
  Nan::MakeCallback(info.This(), cb, argc, argv);

  info.GetReturnValue().SetUndefined();
}

void Init(v8::Handle<v8::Object> exports, v8::Handle<v8::Object> module) {
  Nan::SetMethod(module
    , "exports"
    , RunCallback
  );
}

NODE_MODULE(addon, Init)
