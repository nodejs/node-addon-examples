#define BUILDING_NODE_EXTENSION
#include <node.h>

using namespace v8;

Handle<Value> CreateObject(const Arguments& args) {
  HandleScope scope;

  Local<Object> obj = Object::New();
  obj->Set(String::NewSymbol("msg"), args[0]->ToString());

  return scope.Close(obj);
}

void Init(Handle<Object> target) {
  target->Set(String::NewSymbol("exports"),
      FunctionTemplate::New(CreateObject)->GetFunction());
}

NODE_MODULE_INIT(addon, Init)
