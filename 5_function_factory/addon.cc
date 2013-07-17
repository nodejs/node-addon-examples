#include <node.h>

using namespace v8;

template<class T> void MyFunction(const v8::FunctionCallbackInfo<T>& info) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  info.GetReturnValue().Set(String::New("hello world"));
}

template<class T> void CreateFunction(const v8::FunctionCallbackInfo<T>& info) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  Local<FunctionTemplate> tpl = FunctionTemplate::New(MyFunction);
  Local<Function> fn = tpl->GetFunction();

  // omit this to make it anonymous
  fn->SetName(String::NewSymbol("theFunction"));

  info.GetReturnValue().Set(fn);
}

void Init(Handle<Object> exports, Handle<Object> module) {
  module->Set(String::NewSymbol("exports"),
      FunctionTemplate::New(CreateFunction)->GetFunction());
}

NODE_MODULE(addon, Init)
