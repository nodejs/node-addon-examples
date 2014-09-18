#include <nan.h>
#include "myobject.h"

using namespace v8;

MyObject::MyObject() {};
MyObject::~MyObject() {};

Persistent<Function> MyObject::constructor;

void MyObject::Init() {
  NanScope();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("MyObject"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(NanNew("plusOne"),
      NanNew<FunctionTemplate>(PlusOne)->GetFunction());

  NanAssignPersistent(constructor, tpl->GetFunction());
}

NAN_METHOD(MyObject::New) {
  NanScope();

  MyObject* obj = new MyObject();
  obj->counter_ = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
  obj->Wrap(args.This());

  NanReturnValue(args.This());
}


Local<Object> MyObject::NewInstance(Local<Value> arg) {
  NanEscapableScope();

  const unsigned argc = 1;
  Local<Value> argv[argc] = { arg };
  Local<Function> cons = NanNew<Function>(constructor);
  Local<Object> instance = cons->NewInstance(argc, argv);

  return NanEscapeScope(instance);
}

NAN_METHOD(MyObject::PlusOne) {
  NanScope();

  MyObject* obj = ObjectWrap::Unwrap<MyObject>(args.This());
  obj->counter_ += 1;

  NanReturnValue(NanNew(obj->counter_));
}
