#include <node.h>
#include "pi_est.h"
#include "async.h"

using namespace v8;

// libuv allows us to pass around a pointer to an arbitrary
// object when running asynchronous functions. We create a
// data structure to hold the data we need during and after
// the async work.
typedef struct AsyncData {
  int points;                    // estimation points
  Persistent<Function> callback; // callback function
  double estimate;               // estimation result
} AsyncData;

// Function to execute inside the worker-thread.
// It is not safe to access V8, or V8 data structures
// here, so everything we need for input and output
// should go on our req->data object.
void AsyncWork(uv_work_t *req) {
  // fetch our data structure
  AsyncData *asyncData = (AsyncData *)req->data;
  // run Estimate() and assign the result to our data structure
  asyncData->estimate = Estimate(asyncData->points);
}

// Function to execute when the async work is complete
// this function will be run inside the main event loop
// so it is safe to use V8 again
void AsyncAfter(uv_work_t *req) {
  HandleScope scope;

  // fetch our data structure
  AsyncData *asyncData = (AsyncData *)req->data;
  // create an arguments array for the callback
  Handle<Value> argv[] = {
    Null(),
    Number::New(asyncData->estimate)
  };

  // surround in a try/catch for safety
  TryCatch try_catch;
  // execute the callback function
  asyncData->callback->Call(Context::GetCurrent()->Global(), 2, argv);
  if (try_catch.HasCaught())
    node::FatalException(try_catch);

  // dispose the Persistent handle so the callback
  // function can be garbage-collected
  asyncData->callback.Dispose();
  // clean up any memory we allocated
  delete asyncData;
  delete req;
}

// Asynchronous access to the `Estimate()` function
Handle<Value> CalculateAsync(const Arguments& args) {
  HandleScope scope;

  // create an async work token
  uv_work_t *req = new uv_work_t;
  // assign our data structure that will be passed around
  AsyncData *asyncData = new AsyncData;
  req->data = asyncData;

  // expect a number as the first argument
  asyncData->points = args[0]->Uint32Value();
  // expect a function as the second argument
  // we create a Persistent reference to it so
  // it won't be garbage-collected
  asyncData->callback = Persistent<Function>::New(
      Local<Function>::Cast(args[1]));

  // pass the work token to libuv to be run when a
  // worker-thread is available to
  uv_queue_work(
    uv_default_loop(),
    req,                          // work token
    AsyncWork,                    // work function
    (uv_after_work_cb)AsyncAfter  // function to run when complete
  );

  return scope.Close(Undefined());
}
