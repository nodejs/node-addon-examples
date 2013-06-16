#include <node.h>

using namespace v8;

Handle<Value> RunCallback(const Arguments& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  Local<Function> cb = Local<Function>::Cast(args[0]);
  const unsigned argc = 1;
  Local<Value> argv[argc] = { String::New("hello world") };
  cb->Call(Context::GetCurrent()->Global(), argc, argv);

  return scope.Close(Undefined(isolate));
}

void Init(Handle<Object> exports, Handle<Object> module) {
  module->Set(String::NewSymbol("exports"),
      FunctionTemplate::New(RunCallback)->GetFunction());
}

NODE_MODULE(addon, Init)
