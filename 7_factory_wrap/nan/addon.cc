#include <nan.h>
#include "myobject.h"

NAN_METHOD(CreateObject) {
  info.GetReturnValue().Set(MyObject::NewInstance(info[0]));
}

void InitAll(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  Nan::HandleScope scope;

  MyObject::Init(exports);

  module->Set(Nan::New<v8::String>("exports").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(CreateObject)->GetFunction());
}

NODE_MODULE(addon, InitAll)
