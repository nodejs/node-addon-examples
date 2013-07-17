#include <node.h>
#include "myobject.h"

using namespace v8;

template<class T> void CreateObject(const v8::FunctionCallbackInfo<T>& info) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  MyObject::NewInstance(info);
}

void InitAll(Handle<Object> exports, Handle<Object> module) {
  MyObject::Init();

  module->Set(String::NewSymbol("exports"),
      FunctionTemplate::New(CreateObject)->GetFunction());
}

NODE_MODULE(addon, InitAll)
