
#include <nan.h>

#include "myobject.hpp"

NAN_METHOD(createObject);  ///< object factory
NAN_METHOD(add);           ///< object consumer

NAN_METHOD(createObject)
{
    info.GetReturnValue().Set(MyObject::NewInstance(info[0]));
}

NAN_METHOD(add)
{
    MyObject* obj1 = Nan::ObjectWrap::Unwrap<MyObject>(info[0]->ToObject());
    MyObject* obj2 = Nan::ObjectWrap::Unwrap<MyObject>(info[1]->ToObject());

    double sum = obj1->val() + obj2->val();

    info.GetReturnValue().Set(Nan::New(sum));
}

// module initialization

NAN_MODULE_INIT(init)
{
    MyObject::init(target);

    Nan::Set(target,
             Nan::New("createObject").ToLocalChecked(),
             Nan::GetFunction(Nan::New<v8::FunctionTemplate>(createObject)).ToLocalChecked());
    Nan::Set(target,
             Nan::New("add").ToLocalChecked(),
             Nan::GetFunction(Nan::New<v8::FunctionTemplate>(add)).ToLocalChecked());
}

NODE_MODULE(addon, init)
