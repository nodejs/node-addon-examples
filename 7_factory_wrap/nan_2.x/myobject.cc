#include <nan.h>
#include "myobject.h"

using namespace v8;

Nan::Persistent<v8::Function> MyObject::constructor;

MyObject::MyObject() {}
MyObject::~MyObject() {};

NAN_MODULE_INIT(MyObject::Init) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New<String>("MyObject").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeTemplate(tpl,
      "plusOne",
      Nan::New<FunctionTemplate>(PlusOne)->GetFunction()
  );

  constructor.Reset( tpl->GetFunction() );
}

NAN_METHOD(MyObject::New) {
  MyObject* obj = new MyObject();
  obj->counter_ = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}


Local<Object> MyObject::NewInstance(Local<Value> arg) {
  Nan::EscapableHandleScope scope;

  const unsigned argc = 1;
  Local<Value> argv[argc] = { arg };
  v8::Local<v8::Function> cons = Nan::New<Function>(constructor);
  Local<Object> instance = cons->NewInstance(argc, argv);

  return scope.Escape(instance);
}

NAN_METHOD(MyObject::PlusOne) {
  MyObject* obj = ObjectWrap::Unwrap<MyObject>(info.This());
  obj->counter_ += 1;

  info.GetReturnValue().Set(
    Nan::New<v8::Number>(obj->counter_)
  );
}
