#include "myobject.h"
#include <nan.h>

void InitAll(v8::Local<v8::Object> exports) { MyObject::Init(exports); }

NODE_MODULE(addon, InitAll)
