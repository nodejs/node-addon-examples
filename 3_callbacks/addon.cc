#include <node.h>

Isolate* isolate = Isolate::GetCurrent();

using namespace v8;

Handle<Value> RunCallback(const Arguments& args) {
  HandleScope scope(isolate);

  Local<Function> cb = Local<Function>::Cast(args[0]);
  const unsigned argc = 1;
  Local<Value> argv[argc] = { Local<Value>::New(String::New("hello world")) };
  cb->Call(Context::GetCurrent()->Global(), argc, argv);

  return scope.Close(Undefined(isolate));
}

void Init(Handle<Object> exports, Handle<Object> module) {
  module->Set(String::NewSymbol("exports"),
      FunctionTemplate::New(RunCallback)->GetFunction());
}

NODE_MODULE(addon, Init)
