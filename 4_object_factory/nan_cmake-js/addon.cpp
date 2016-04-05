
#include <nan.h>

NAN_METHOD(createObject)
{
    if (info.Length() != 1)
    {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }
    if (!info[0]->IsString())
    {
        Nan::ThrowTypeError("Wrong argument type");
        return;
    }

    // create an object and add attributes to it
    v8::Local<v8::Object> obj = Nan::New<v8::Object>();
    obj->Set(Nan::New("msg").ToLocalChecked(),
             info[0]->ToString());

    info.GetReturnValue().Set(obj);
}

// module initialization

NAN_MODULE_INIT(init)
{
    Nan::Set(target,
             Nan::New("createObject").ToLocalChecked(),
             Nan::GetFunction(Nan::New<v8::FunctionTemplate>(createObject)).ToLocalChecked());
}

NODE_MODULE(addon, init)
