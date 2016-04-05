
#include "myobject.hpp"

Nan::Persistent<v8::Function> MyObject::constructor;

MyObject::MyObject()
: val_(0)
{
}

MyObject::~MyObject()
{
}

NAN_METHOD(MyObject::New)
{
    MyObject* obj = new MyObject();
    obj->val_ = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
    obj->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

double MyObject::val() const
{
    return val_;
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
    tpl->InstanceTemplate()->SetInternalFieldCount(1);  // 1 attribute: this->val_

    constructor.Reset(tpl->GetFunction());
}
