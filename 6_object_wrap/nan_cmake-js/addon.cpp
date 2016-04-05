#include <nan.h>
#include "myobject.hpp"

void InitAll(v8::Local<v8::Object> exports) {
  MyObject::Init(exports);
}

NODE_MODULE(addon, InitAll)
