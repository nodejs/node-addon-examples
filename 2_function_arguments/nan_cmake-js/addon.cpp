
#include <nan.h>

NAN_METHOD(add)
{
    if (info.Length() != 2)
    {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }
    // @see https://v8docs.nodesource.com/node-5.0/dc/d0a/classv8_1_1_value.html
    if (!info[0]->IsNumber() || !info[1]->IsNumber())
    {
        Nan::ThrowTypeError("Wrong argument types");
        return;
    }

    // get the arguments and compute the result
    // @see https://v8docs.nodesource.com/node-5.0/dc/d0a/classv8_1_1_value.html
    double arg0 = info[0]->NumberValue();
    double arg1 = info[1]->NumberValue();
    v8::Local<v8::Number> num = Nan::New(arg0 + arg1);

    // in C++11, the return type can be deduced by the compiler
    // @see https://github.com/nodejs/nan/blob/master/doc%2Fnew.md#nannew
    // auto num = Nan::New(arg0 + arg1);

    info.GetReturnValue().Set(num);
}

// module initialization

NAN_MODULE_INIT(init)
{
    Nan::Set(target,
             Nan::New("add").ToLocalChecked(),
             Nan::GetFunction(Nan::New<v8::FunctionTemplate>(add)).ToLocalChecked());
}

NODE_MODULE(addon, init)
