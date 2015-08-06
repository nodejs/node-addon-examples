#include <nan.h>

using namespace v8;

NAN_METHOD(CreateObject) {
  NanScope();

  Local<Object> obj = NanNew<Object>();
  obj->Set(NanNew("msg"), args[0]->ToString());

  NanReturnValue(obj);
}

void Init(Handle<Object> exports, Handle<Object> module) {
  module->Set(NanNew("exports"),
      NanNew<FunctionTemplate>(CreateObject)->GetFunction());
}

NODE_MODULE(addon, Init)
