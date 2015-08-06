#include <node.h>
#include "myobject.h"

using namespace v8;

MyObject::MyObject() {};
MyObject::~MyObject() {};

Nan::Persistent<v8::Function> MyObject::constructor;

void MyObject::Init() {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New<String>("MyObject").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor.Reset(tpl->GetFunction());
}

NAN_METHOD(MyObject::New) {
  MyObject* obj = new MyObject();
  obj->val_ = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

Local<Object> MyObject::NewInstance(Local<Value> arg) {
  Nan::EscapableHandleScope scope;

  const unsigned argc = 1;
  Local<Value> argv[argc] = { arg };
  Local<Function> cons = Nan::New<Function>(constructor);
  Local<Object> instance = cons->NewInstance(argc, argv);

  return scope.Escape(instance);
}
