#include <node.h>

using namespace v8;

template<class T> void Add(const v8::FunctionCallbackInfo<T>& info) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (info.Length() < 2) {
    ThrowException(Exception::TypeError(
        String::New("Wrong number of arguments")));
    info.GetReturnValue().Set(Undefined(isolate));
    return;
  }

  if (!info[0]->IsNumber() || !info[1]->IsNumber()) {
    ThrowException(Exception::TypeError(String::New("Wrong arguments")));
    info.GetReturnValue().Set(Undefined(isolate));
    return;
  }

  Local<Number> num = Number::New(info[0]->NumberValue() +
      info[1]->NumberValue());
  info.GetReturnValue().Set(num);
}

void Init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("add"),
      FunctionTemplate::New(Add)->GetFunction());
}

NODE_MODULE(addon, Init)
