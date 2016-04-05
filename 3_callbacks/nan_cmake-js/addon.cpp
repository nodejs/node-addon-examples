
#include <nan.h>

NAN_METHOD(runCallback)
{
    if (info.Length() != 1)
    {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }
    if (!info[0]->IsFunction())
    {
        Nan::ThrowTypeError("Wrong argument type");
        return;
    }

    // prepare the callback function and its arguments
    v8::Local<v8::Function> callbackFunction = info[0].As<v8::Function>();
    const unsigned argc = 1;
    v8::Local<v8::Value> argv[argc] = {
        Nan::New("Hello world!").ToLocalChecked()
    };

    Nan::MakeCallback(Nan::GetCurrentContext()->Global(),
                      callbackFunction,
                      argc,
                      argv);
}

// module initialization

NAN_MODULE_INIT(init)
{
    Nan::Set(target,
             Nan::New("runCallback").ToLocalChecked(),
             Nan::GetFunction(Nan::New<v8::FunctionTemplate>(runCallback)).ToLocalChecked());
}

NODE_MODULE(addon, init)
