#include <node.h>
#include "pi_est.h"
#include "sync.h"

using namespace v8;

// Simple synchronous access to the `Estimate()` function
void CalculateSync(const v8::FunctionCallbackInfo<Value>& info) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  // expect a number as the first argument
  int points = info[0]->Uint32Value();
  double est = Estimate(points);

  info.GetReturnValue().Set(Number::New(est));
}
