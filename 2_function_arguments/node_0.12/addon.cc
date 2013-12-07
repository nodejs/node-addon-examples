#include <node.h>

using namespace v8;

void Add(const v8::FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.Length() < 2) {
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
    return;
  }

  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(String::New("Wrong arguments")));
    return;
  }

  double arg0 = args[0]->NumberValue();
  double arg1 = args[1]->NumberValue();
  Local<Number> num = Number::New(arg0 + arg1);

  args.GetReturnValue().Set(num);
}

void Init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("add"),
      FunctionTemplate::New(Add)->GetFunction());
}

NODE_MODULE(addon, Init)
