#include <node.h>

using namespace v8;

Handle<Value> MyFunction(const Arguments& args) {
  HandleScope scope;
  return scope.Close(String::New("hello world"));
}

Handle<Value> CreateFunction(const Arguments& args) {
  HandleScope scope;

  Local<FunctionTemplate> tpl = FunctionTemplate::New(MyFunction);
  Local<Function> fn = tpl->GetFunction();
  fn->SetName(String::NewSymbol("theFunction")); // omit this to make it anonymous

  return scope.Close(fn);
}

void Init(Handle<Object> exports, Handle<Object> module) {
  module->Set(String::NewSymbol("exports"),
      FunctionTemplate::New(CreateFunction)->GetFunction());
}

NODE_MODULE(addon, Init)
