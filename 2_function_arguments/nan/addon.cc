#include <nan.h>

using namespace v8;

NAN_METHOD(Add) {
  NanScope();

  if (args.Length() < 2) {
    NanThrowTypeError("Wrong number of arguments");
    NanReturnUndefined();
  }

  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    NanThrowTypeError("Wrong arguments");
    NanReturnUndefined();
  }

  double arg0 = args[0]->NumberValue();
  double arg1 = args[1]->NumberValue();
  Local<Number> num = NanNew(arg0 + arg1);

  NanReturnValue(num);
}

void Init(Handle<Object> exports) {
  exports->Set(NanNew("add"), NanNew<FunctionTemplate>(Add)->GetFunction());
}

NODE_MODULE(addon, Init)
