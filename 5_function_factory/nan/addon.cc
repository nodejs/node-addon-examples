#include <nan.h>

NAN_METHOD(MyFunction) {
  info.GetReturnValue().Set(Nan::New("hello world").ToLocalChecked());
}

NAN_METHOD(CreateFunction) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(MyFunction);
  v8::Local<v8::Function> fn = tpl->GetFunction();

  // omit this to make it anonymous
  fn->SetName(Nan::New<v8::String>("theFunction").ToLocalChecked());

  info.GetReturnValue().Set(fn);
}

void Init(v8::Handle<v8::Object> exports, v8::Handle<v8::Object> module) {
  Nan::SetMethod(module, "exports", CreateFunction);
}

NODE_MODULE(addon, Init)
