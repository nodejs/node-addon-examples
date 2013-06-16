#include <node.h>

using namespace v8;

Handle<Value> MyFunction(const Arguments& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  return scope.Close(String::New("hello world"));
}

Handle<Value> CreateFunction(const Arguments& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  Local<FunctionTemplate> tpl = FunctionTemplate::New(MyFunction);
  Local<Function> fn = tpl->GetFunction();

  // omit this to make it anonymous
  fn->SetName(String::NewSymbol("theFunction"));

  return scope.Close(fn);
}

void Init(Handle<Object> exports, Handle<Object> module) {
  module->Set(String::NewSymbol("exports"),
      FunctionTemplate::New(CreateFunction)->GetFunction());
}

NODE_MODULE(addon, Init)
