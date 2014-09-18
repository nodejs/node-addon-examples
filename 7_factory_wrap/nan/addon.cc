#include <nan.h>
#include "myobject.h"

using namespace v8;

NAN_METHOD(CreateObject) {
  NanScope();
  NanReturnValue(MyObject::NewInstance(args[0]));
}

void InitAll(Handle<Object> exports, Handle<Object> module) {
  NanScope();

  MyObject::Init();

  module->Set(NanNew("exports"),
      NanNew<FunctionTemplate>(CreateObject)->GetFunction());
}

NODE_MODULE(addon, InitAll)
