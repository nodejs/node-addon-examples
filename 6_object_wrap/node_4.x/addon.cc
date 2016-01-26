// addon.cc
#include <node.h>
#include "myobject.h"

namespace demo {

using v8::Local;

void InitAll(Local<Object> exports) {
  MyObject::Init(exports);
}

NODE_MODULE(addon, InitAll)

} // namespace demo
