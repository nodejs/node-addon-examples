#include <node.h>
#include "myobject.h"

Isolate* isolate = Isolate::GetCurrent();

using namespace v8;

Handle<Value> CreateObject(const Arguments& args) {
  HandleScope scope(isolate);
  return scope.Close(MyObject::NewInstance(args));
}

void InitAll(Handle<Object> exports, Handle<Object> module) {
  MyObject::Init();

  module->Set(String::NewSymbol("exports"),
      FunctionTemplate::New(CreateObject)->GetFunction());
}

NODE_MODULE(addon, InitAll)
