
#include "myobject.hpp"

Nan::Persistent<v8::Function> MyObject::constructor;

MyObject::MyObject(double value)
: value_(value)
{
}

MyObject::~MyObject()
{
}

NAN_METHOD(MyObject::New)
{
    if (info.IsConstructCall())
    {
        // invoked as constructor: `new MyObject(...)`
        double value = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
        MyObject* obj = new MyObject(value);
        obj->Wrap(info.This());

        info.GetReturnValue().Set(info.This());
    }
    else
    {
        // invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { info[0] };
        v8::Local<v8::Function> ctor = Nan::New<v8::Function>(constructor);

        info.GetReturnValue().Set(ctor->NewInstance(argc, argv));
    }
}

NAN_METHOD(MyObject::getValue)
{
    MyObject* obj = ObjectWrap::Unwrap<MyObject>(info.Holder());

    info.GetReturnValue().Set(Nan::New(obj->value_));
}

NAN_METHOD(MyObject::plusOne)
{
    MyObject* obj = Nan::ObjectWrap::Unwrap<MyObject>(info.Holder());

    obj->value_ += 1;

    info.GetReturnValue().Set(Nan::New(obj->value_));
}

NAN_METHOD(MyObject::multiply)
{
    MyObject* obj = ObjectWrap::Unwrap<MyObject>(info.Holder());

    // compute the result of the multiplication
    double multiple = info[0]->IsUndefined() ? 1 : info[0]->NumberValue();
    double result   = obj->value_ * multiple;

    // use the result to create the new object
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = { Nan::New(result) };
    v8::Local<v8::Function> ctor = Nan::New<v8::Function>(constructor);

    info.GetReturnValue().Set(ctor->NewInstance(argc, argv));
}

NAN_MODULE_INIT(MyObject::init)
{
    Nan::HandleScope scope;

    // prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("MyObject").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);  // 1 attribute: this->value_

    // prototype
    Nan::SetPrototypeMethod(tpl, "getValue", getValue);
    Nan::SetPrototypeMethod(tpl, "plusOne", plusOne);
    Nan::SetPrototypeMethod(tpl, "multiply", multiply);

    constructor.Reset(tpl->GetFunction());

    Nan::Set(target,
             Nan::New("MyObject").ToLocalChecked(),
             Nan::GetFunction(tpl).ToLocalChecked());
}
