#include <nan.h>
#include "myobject.h"

using namespace v8;

NAN_METHOD(CreateObject) {
  info.GetReturnValue().Set(MyObject::NewInstance(info[0]));
}

NAN_METHOD(Add) {
  MyObject* obj1 = Nan::ObjectWrap::Unwrap<MyObject>(info[0]->ToObject());
  MyObject* obj2 = Nan::ObjectWrap::Unwrap<MyObject>(info[1]->ToObject());
  double sum = obj1->Val() + obj2->Val();
  info.GetReturnValue().Set(Nan::New(sum));
}

void InitAll(v8::Handle<v8::Object> exports) {
  MyObject::Init();

  Nan::Set(exports,
    Nan::New<v8::String>("createObject").ToLocalChecked(),
    Nan::New<v8::FunctionTemplate>(CreateObject)->GetFunction()
  );

  Nan::Set(exports,
      Nan::New<v8::String>("add").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(Add)->GetFunction()
  );
}

NODE_MODULE(addon, InitAll)
