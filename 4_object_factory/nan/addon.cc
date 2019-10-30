#include <nan.h>

void CreateObject(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
  v8::Local<v8::Object> obj = Nan::New<v8::Object>();
  obj->Set(context,
           Nan::New("msg").ToLocalChecked(),
           info[0]->ToString(context).ToLocalChecked());

  info.GetReturnValue().Set(obj);
}

void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  v8::Local<v8::Context> context = exports->CreationContext();
  module->Set(context,
              Nan::New("exports").ToLocalChecked(),
              Nan::New<v8::FunctionTemplate>(CreateObject)
                  ->GetFunction(context)
                  .ToLocalChecked());
}

NODE_MODULE(addon, Init)
