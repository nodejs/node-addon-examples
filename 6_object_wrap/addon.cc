#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "myobject.h"

using namespace v8;

void InitAll(Handle<Object> target) {
  MyObject::Init(target);
}

NODE_MODULE_INIT(addon, InitAll)
