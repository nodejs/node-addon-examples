#include <nan.h>
#include "async.h"  // NOLINT(build/include)
#include "sync.h"   // NOLINT(build/include)

using Nan::GetFunction;
using Nan::New;
using Nan::Set;
using v8::FunctionTemplate;
using v8::Object;
using v8::String;

// Expose synchronous and asynchronous access to our
// Estimate() function
NAN_MODULE_INIT(InitAll) {
  Set(target,
      New<String>("calculateSync").ToLocalChecked(),
      GetFunction(New<FunctionTemplate>(CalculateSync)).ToLocalChecked());

  Set(target,
      New<String>("calculateAsync").ToLocalChecked(),
      GetFunction(New<FunctionTemplate>(CalculateAsync)).ToLocalChecked());
}

NODE_MODULE(addon, InitAll)
