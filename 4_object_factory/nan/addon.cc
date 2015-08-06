#include <nan.h>

void CreateObject(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> obj = Nan::New<v8::Object>();
  obj->Set(Nan::New("msg").ToLocalChecked(), info[0]->ToString());

  info.GetReturnValue().Set(obj);
}

void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  module->Set(Nan::New("exports").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(CreateObject)->GetFunction());
}

NODE_MODULE(addon, Init)
