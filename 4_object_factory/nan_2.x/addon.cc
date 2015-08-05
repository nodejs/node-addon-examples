#include <nan.h>

using namespace v8;

NAN_METHOD(CreateObject) {
  Local<Object> obj = Nan::New<Object>();
  obj->Set(
    Nan::New<v8::String>("msg").ToLocalChecked()
    , info[0]->ToString()
  );

  info.GetReturnValue().Set(obj);
}

void Init(Handle<Object> exports, Handle<Object> module) {
  Nan::SetMethod(module
    , "exports"
    , CreateObject
  );
}

NODE_MODULE(addon, Init)
