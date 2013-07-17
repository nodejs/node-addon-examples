#include <node.h>

using namespace v8;

template<class T> void CreateObject(const v8::FunctionCallbackInfo<T>& info) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  Local<Object> obj = Object::New();
  obj->Set(String::NewSymbol("msg"), info[0]->ToString());

  info.GetReturnValue().Set(obj);
}

void Init(Handle<Object> exports, Handle<Object> module) {
  module->Set(String::NewSymbol("exports"),
      FunctionTemplate::New(CreateObject)->GetFunction());
}

NODE_MODULE(addon, Init)
