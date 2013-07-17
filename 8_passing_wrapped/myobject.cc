#include <node.h>
#include "myobject.h"

using namespace v8;

Isolate* isolate = Isolate::GetCurrent();

MyObject::MyObject() {};
MyObject::~MyObject() {};

Persistent<Function> MyObject::constructor;

void MyObject::Init() {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("MyObject"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor.Reset(isolate, tpl->GetFunction());
}

template<class T> void MyObject::New(const v8::FunctionCallbackInfo<T>& info) {
  HandleScope scope(isolate);

  MyObject* obj = new MyObject();
  obj->val_ = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

void MyObject::NewInstance(const v8::FunctionCallbackInfo<Value>& info) {
  HandleScope scope(isolate);

  const unsigned argc = 1;
  Handle<Value> argv[argc] = { info[0] };
  Local<Object> instance = Local<Function>::New(isolate, constructor)->NewInstance(argc, argv);

  info.GetReturnValue().Set(instance);
}
