#include <nan.h>

using namespace v8;
using namespace Nan;

NAN_METHOD(Add) {

  if (info.Length() < 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsNumber() || !info[1]->IsNumber()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  double arg0 = info[0]->NumberValue();
  double arg1 = info[1]->NumberValue();

  info.GetReturnValue().Set(arg0 + arg1);
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target
    , Nan::New<v8::String>("add").ToLocalChecked()
    , Nan::New<v8::FunctionTemplate>(Add)->GetFunction()
  );
}

NODE_MODULE(addon, Init)
