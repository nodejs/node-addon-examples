#include <nan.h>
#include "myobject.h"

using namespace v8;

NAN_METHOD(CreateObject) {
  info.GetReturnValue().Set(MyObject::NewInstance(info[0]));
}

NAN_METHOD(Add) {

  MyObject* obj1 = node::ObjectWrap::Unwrap<MyObject>(
      info[0]->ToObject());
  MyObject* obj2 = node::ObjectWrap::Unwrap<MyObject>(
      info[1]->ToObject());

  double sum = obj1->Val() + obj2->Val();
  info.GetReturnValue().Set(sum);
}

void InitAll(Handle<Object> exports) {
  MyObject::Init();

  Nan::Set(exports,
    Nan::New<String>("createObject").ToLocalChecked(),
    Nan::New<FunctionTemplate>(CreateObject)->GetFunction()
  );

  Nan::Set(exports,
    Nan::New<String>("add").ToLocalChecked(),
    Nan::New<FunctionTemplate>(Add)->GetFunction()
  );
}

NODE_MODULE(addon, InitAll)
