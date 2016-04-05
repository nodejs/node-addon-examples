
#include <nan.h>

#include "myobject.hpp"

Nan::Persistent<v8::Function> MyObject::constructor;

MyObject::MyObject()
: counter_(0)
{
}

MyObject::~MyObject()
{
}

NAN_METHOD(MyObject::New)
{
    MyObject* obj = new MyObject();
    obj->counter_ = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
    obj->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(MyObject::plusOne)
{
    MyObject* obj = ObjectWrap::Unwrap<MyObject>(info.This());

    obj->counter_ += 1;

    info.GetReturnValue().Set(Nan::New(obj->counter_));
}

v8::Local<v8::Object> MyObject::NewInstance(v8::Local<v8::Value> arg)
{
    Nan::EscapableHandleScope scope;

    const unsigned argc = 1;
    v8::Local<v8::Value> argv[argc] = { arg };
    v8::Local<v8::Function> ctor = Nan::New<v8::Function>(constructor);
    v8::Local<v8::Object> instance = ctor->NewInstance(argc, argv);

    return scope.Escape(instance);
}

NAN_MODULE_INIT(MyObject::init)
{
    Nan::HandleScope scope;

    // prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("MyObject").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);  // 1 attribute: this->counter_

    // prototype
    //tpl->PrototypeTemplate()->Set(Nan::New("plusOne").ToLocalChecked(),
    //                              Nan::New<v8::FunctionTemplate>(plusOne)->GetFunction());
    Nan::SetPrototypeMethod(tpl, "plusOne", plusOne);

    constructor.Reset(tpl->GetFunction());

    // This class' constructor won't be added to the `target` object.
    // (we want this class to be "constructible" only through the
    // `createObject()` factory method)
    // As a result, the `target` object is of no use here.
}
