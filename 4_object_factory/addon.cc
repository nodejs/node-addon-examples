#include <node.h>

using namespace v8;

Handle<Value> CreateObject(const Arguments& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  Local<Object> obj = Object::New();
  obj->Set(String::NewSymbol("msg"), args[0]->ToString());

  return scope.Close(obj);
}

void Init(Handle<Object> exports, Handle<Object> module) {
  module->Set(String::NewSymbol("exports"),
      FunctionTemplate::New(CreateObject)->GetFunction());
}

NODE_MODULE(addon, Init)
