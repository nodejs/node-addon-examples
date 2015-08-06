#include <nan.h>

using namespace v8;

NAN_METHOD(MyFunction) {
  NanScope();
  NanReturnValue(NanNew("hello world"));
}

NAN_METHOD(CreateFunction) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(MyFunction);
  Local<Function> fn = tpl->GetFunction();

  // omit this to make it anonymous
  fn->SetName(NanNew("theFunction"));

  NanReturnValue(fn);
}

void Init(Handle<Object> exports, Handle<Object> module) {
  NODE_SET_METHOD(module, "exports", CreateFunction);
}

NODE_MODULE(addon, Init)
