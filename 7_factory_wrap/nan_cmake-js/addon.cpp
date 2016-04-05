
#include <nan.h>

#include "myobject.hpp"

NAN_METHOD(createObject)
{
    if (info.Length() != 1)
    {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }
    if (!info[0]->IsNumber())
    {
        Nan::ThrowTypeError("Wrong argument type");
        return;
    }

    info.GetReturnValue().Set(MyObject::NewInstance(info[0]));
}

// module initialization

NAN_MODULE_INIT(init)
{
    Nan::HandleScope scope;

    MyObject::init(target);

    Nan::Set(target,
             Nan::New("createObject").ToLocalChecked(),
             Nan::GetFunction(Nan::New<v8::FunctionTemplate>(createObject)).ToLocalChecked());
}

NODE_MODULE(addon, init)
