#include "myobject.h"

Nan::Persistent<v8::Function> MyObject::constructor;

MyObject::MyObject(double value) : value_(value) {}

MyObject::~MyObject() {}

void MyObject::Init(v8::Local<v8::Object> exports) {
  v8::Local<v8::Context> context =
      exports->GetCreationContext().ToLocalChecked();
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("MyObject").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // define a filed with getter and setter
  Nan::SetAccessor(tpl->InstanceTemplate(),
                   Nan::New("value").ToLocalChecked(),
                   GetValueAcc,
                   SetValueAcc);
  Nan::SetPrototypeMethod(tpl, "getValue", GetValue);
  Nan::SetPrototypeMethod(tpl, "plusOne", PlusOne);
  Nan::SetPrototypeMethod(tpl, "multiply", Multiply);

  constructor.Reset(tpl->GetFunction(context).ToLocalChecked());
  exports->Set(context,
               Nan::New("MyObject").ToLocalChecked(),
               tpl->GetFunction(context).ToLocalChecked());
}

void MyObject::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
  if (info.IsConstructCall()) {
    // Invoked as constructor: `new MyObject(...)`
    double value =
        info[0]->IsUndefined() ? 0 : info[0]->NumberValue(context).FromJust();
    MyObject* obj = new MyObject(value);
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = {info[0]};
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(
        cons->NewInstance(context, argc, argv).ToLocalChecked());
  }
}
/**
 * Nan Property Getter
 */
void MyObject::GetValueAcc(v8::Local<v8::String> property,
                           const Nan::PropertyCallbackInfo<v8::Value>& info) {
  MyObject* obj = ObjectWrap::Unwrap<MyObject>(info.Holder());
  info.GetReturnValue().Set(Nan::New(obj->value_));
}
/**
 * Nan Property Setter
 */
void MyObject::SetValueAcc(v8::Local<v8::String> property,
                           v8::Local<v8::Value> value,
                           const Nan::PropertyCallbackInfo<void>& info) {
  MyObject* obj = ObjectWrap::Unwrap<MyObject>(info.Holder());
  obj->value_ =
      value->NumberValue(info.GetIsolate()->GetCurrentContext()).FromJust();
}

void MyObject::GetValue(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  MyObject* obj = ObjectWrap::Unwrap<MyObject>(info.Holder());
  info.GetReturnValue().Set(Nan::New(obj->value_));
}

void MyObject::PlusOne(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  MyObject* obj = ObjectWrap::Unwrap<MyObject>(info.Holder());
  obj->value_ += 1;
  info.GetReturnValue().Set(Nan::New(obj->value_));
}

void MyObject::Multiply(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
  MyObject* obj = ObjectWrap::Unwrap<MyObject>(info.Holder());
  double multiple =
      info[0]->IsUndefined() ? 1 : info[0]->NumberValue(context).FromJust();

  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);

  const int argc = 1;
  v8::Local<v8::Value> argv[argc] = {Nan::New(obj->value_ * multiple)};

  info.GetReturnValue().Set(
      cons->NewInstance(context, argc, argv).ToLocalChecked());
}
