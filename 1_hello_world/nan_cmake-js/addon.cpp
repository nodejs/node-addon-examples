
// based on https://github.com/fcanas/node-native-boilerplate

#include <nan.h>

// method declaration + definition

NAN_METHOD(sayHello);  // this should go into a header file

NAN_METHOD(sayHello)   // and this should be in a separate source file
{
    info.GetReturnValue().Set(Nan::New("Hello world!").ToLocalChecked());
}

// module initialization

NAN_MODULE_INIT(init)
{
    Nan::Set(target,
             Nan::New("sayHello").ToLocalChecked(),
             Nan::GetFunction(Nan::New<v8::FunctionTemplate>(sayHello)).ToLocalChecked());
}

NODE_MODULE(addon, init)
