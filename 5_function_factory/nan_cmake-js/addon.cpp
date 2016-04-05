
#include <nan.h>

NAN_METHOD(myFunction);      ///< the function to be instantiated
NAN_METHOD(createFunction);  ///< the function factory

NAN_METHOD(myFunction)
{
    info.GetReturnValue().Set(Nan::New("Hello world!").ToLocalChecked());
}

NAN_METHOD(createFunction)
{
    // create a new function from myFunction's template
    v8::Local<v8::FunctionTemplate> functionTemplate = Nan::New<v8::FunctionTemplate>(myFunction);
    v8::Local<v8::Function> functionInstance = functionTemplate->GetFunction();

    // omit this to make it anonymous
    functionInstance->SetName(Nan::New("theFunction").ToLocalChecked());

    info.GetReturnValue().Set(functionInstance);
}

// module initialization

NAN_MODULE_INIT(init)
{
    Nan::Set(target,
             Nan::New("createFunction").ToLocalChecked(),
             Nan::GetFunction(Nan::New<v8::FunctionTemplate>(createFunction)).ToLocalChecked());
}

NODE_MODULE(addon, init)
