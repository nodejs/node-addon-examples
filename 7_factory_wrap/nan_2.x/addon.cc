#include <nan.h>
#include "myobject.h"

using namespace v8;

NAN_METHOD(CreateObject) {
  info.GetReturnValue().Set(MyObject::NewInstance(info[0]));
}

void InitAll(Handle<Object> exports, Handle<Object> module) {

  MyObject::Init(exports);

  Nan::SetMethod(module
    , "exports"
    , CreateObject
  );
}

NODE_MODULE(addon, InitAll)
