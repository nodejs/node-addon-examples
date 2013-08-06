#include <node.h>

using namespace v8;

template<class T> void RunCallback(const v8::FunctionCallbackInfo<T>& info) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  Local<Function> cb = Local<Function>::Cast(info[0]);
  const unsigned argc = 1;
  Local<Value> argv[argc] = { String::New("hello world") };
  cb->Call(Context::GetCurrent()->Global(), argc, argv);

  info.GetReturnValue().SetUndefined();
}

void Init(Handle<Object> exports, Handle<Object> module) {
  module->Set(String::NewSymbol("exports"),
      FunctionTemplate::New(RunCallback)->GetFunction());
}

NODE_MODULE(addon, Init)
