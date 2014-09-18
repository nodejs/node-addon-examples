#include <node.h>
#include "myobject.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
  MyObject::Init(exports);
}

NODE_MODULE(addon, InitAll)
