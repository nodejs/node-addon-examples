#include <nan.h>
#include "myobject.h"

using namespace v8;

void CreateObject(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  info.GetReturnValue().Set(MyObject::NewInstance(info[0]));
}

void Add(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();

  MyObject* obj1 = Nan::ObjectWrap::Unwrap<MyObject>(
      info[0]->ToObject(context).ToLocalChecked());
  MyObject* obj2 = Nan::ObjectWrap::Unwrap<MyObject>(
      info[1]->ToObject(context).ToLocalChecked());
  double sum = obj1->Val() + obj2->Val();
  info.GetReturnValue().Set(Nan::New(sum));
}

void InitAll(v8::Local<v8::Object> exports) {
  v8::Local<v8::Context> context = exports->CreationContext();

  MyObject::Init();

  exports->Set(context,
               Nan::New("createObject").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(CreateObject)
                   ->GetFunction(context)
                   .ToLocalChecked());

  exports->Set(context,
               Nan::New("add").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Add)
                   ->GetFunction(context)
                   .ToLocalChecked());
}

NODE_MODULE(addon, InitAll)
