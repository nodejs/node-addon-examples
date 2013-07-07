#include <node.h>
#include "pi_est.h"
#include "sync.h"

using namespace v8;

// Simple synchronous access to the `Estimate()` function
Handle<Value> CalculateSync(const Arguments& args) {
  HandleScope scope;

  // expect a number as the first argument
  int points = args[0]->Uint32Value();
  double est = Estimate(points);

  return scope.Close(Number::New(est));
}
