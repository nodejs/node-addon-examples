#include <nan.h>
#include "myobject.h"

void CreateObject(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  info.GetReturnValue().Set(MyObject::NewInstance(info[0]));
}

void InitAll(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  v8::Local<v8::Context> context = exports->CreationContext();

  Nan::HandleScope scope;

  MyObject::Init();

  module->Set(context,
              Nan::New("exports").ToLocalChecked(),
              Nan::New<v8::FunctionTemplate>(CreateObject)
                  ->GetFunction(context)
                  .ToLocalChecked());
}

NODE_MODULE(addon, InitAll)
