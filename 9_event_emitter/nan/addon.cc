#include <nan.h>
#include "timer.h"

void InitAll(v8::Local<v8::Object> exports) {
  Timer::Init(exports);
}

NODE_MODULE(addon, InitAll)
