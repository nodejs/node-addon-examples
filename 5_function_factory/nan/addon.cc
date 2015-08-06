#include <nan.h>

void MyFunction(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  info.GetReturnValue().Set(Nan::New("hello world").ToLocalChecked());
}

void CreateFunction(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(MyFunction);
  v8::Local<v8::Function> fn = tpl->GetFunction();

  // omit this to make it anonymous
  fn->SetName(Nan::New("theFunction").ToLocalChecked());

  info.GetReturnValue().Set(fn);
}

void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  Nan::SetMethod(module, "exports", CreateFunction);
}

NODE_MODULE(addon, Init)
