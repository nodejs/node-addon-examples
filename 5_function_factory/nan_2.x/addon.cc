#include <nan.h>

using namespace v8;

NAN_METHOD(MyFunction) {
  info.GetReturnValue().Set(
    Nan::New<v8::String>("hello world").ToLocalChecked()
  );
}

NAN_METHOD(CreateFunction) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(MyFunction);
  Local<Function> fn = tpl->GetFunction();

  // omit this to make it anonymous
  fn->SetName(Nan::New<v8::String>("theFunction").ToLocalChecked());

  info.GetReturnValue().Set(fn);
}

void Init(Handle<Object> exports, Handle<Object> module) {
  Nan::SetMethod(module
    , "exports"
    , CreateFunction
  );
}

NODE_MODULE(addon, Init)
