#include <node.h>
#include "myobject.h"

using namespace v8;

template<class T> void CreateObject(const v8::FunctionCallbackInfo<T>& info) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  MyObject::NewInstance(info);
}

template<class T> void Add(const v8::FunctionCallbackInfo<T>& info) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  MyObject* obj1 = node::ObjectWrap::Unwrap<MyObject>(
      info[0]->ToObject());
  MyObject* obj2 = node::ObjectWrap::Unwrap<MyObject>(
      info[1]->ToObject());

  double sum = obj1->Val() + obj2->Val();
  info.GetReturnValue().Set(Number::New(sum));
}

void InitAll(Handle<Object> exports) {
  MyObject::Init();

  exports->Set(String::NewSymbol("createObject"),
      FunctionTemplate::New(CreateObject)->GetFunction());

  exports->Set(String::NewSymbol("add"),
      FunctionTemplate::New(Add)->GetFunction());
}

NODE_MODULE(addon, InitAll)
