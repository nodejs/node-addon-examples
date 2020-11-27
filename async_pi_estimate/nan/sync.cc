#include "sync.h"  // NOLINT(build/include)
#include <nan.h>
#include "pi_est.h"  // NOLINT(build/include)

// Simple synchronous access to the `Estimate()` function
NAN_METHOD(CalculateSync) {
  // expect a number as the first argument
  int points = info[0]->Uint32Value(Nan::GetCurrentContext()).FromJust();
  double est = Estimate(points);

  info.GetReturnValue().Set(est);
}
