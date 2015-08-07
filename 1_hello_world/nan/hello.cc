#include <nan.h>

NAN_METHOD(Method) {
  info.GetReturnValue().Set(Nan::New<v8::String>("world").ToLocalChecked());
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target
    , Nan::New<v8::String>("hello").ToLocalChecked()
    , Nan::New<v8::FunctionTemplate>(Method)->GetFunction()
  );
}

NODE_MODULE(hello, Init)
