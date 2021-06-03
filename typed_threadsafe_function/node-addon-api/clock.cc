#include <chrono>
#include <napi.h>
#include <thread>

using namespace Napi;

using Context = Reference<Value>;
using DataType = int;
void CallJs(Napi::Env env, Function callback, Context *context, DataType *data);
using TSFN = TypedThreadSafeFunction<Context, DataType, CallJs>;
using FinalizerDataType = void;

std::thread nativeThread;
TSFN tsfn;

Value Start(const CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    throw TypeError::New(env, "Expected two arguments");
  } else if (!info[0].IsFunction()) {
    throw TypeError::New(env, "Expected first arg to be function");
  } else if (!info[1].IsNumber()) {
    throw TypeError::New(env, "Expected second arg to be number");
  }

  int count = info[1].As<Number>().Int32Value();

  // Create a new context set to the the receiver (ie, `this`) of the function
  // call
  Context *context = new Reference<Value>(Persistent(info.This()));

  // Create a ThreadSafeFunction
  tsfn = TSFN::New(
      env,
      info[0].As<Function>(), // JavaScript function called asynchronously
      "Resource Name",        // Name
      0,                      // Unlimited queue
      1,                      // Only one thread will use this initially
      context,
      [](Napi::Env, FinalizerDataType *,
         Context *ctx) { // Finalizer used to clean threads up
        nativeThread.join();
        delete ctx;
      });

  // Create a native thread
  nativeThread = std::thread([count] {
    for (int i = 0; i < count; i++) {
      // Create new data
      int *value = new int(clock());

      // Perform a blocking call
      napi_status status = tsfn.BlockingCall(value);
      if (status != napi_ok) {
        // Handle error
        break;
      }

      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Release the thread-safe function
    tsfn.Release();
  });

  return Boolean::New(env, true);
}

// Transform native data into JS data, passing it to the provided
// `callback` -- the TSFN's JavaScript function.
void CallJs(Napi::Env env, Function callback, Context *context,
            DataType *data) {
  // Is the JavaScript environment still available to call into, eg. the TSFN is
  // not aborted
  if (env != nullptr) {
    // On Node-API 5+, the `callback` parameter is optional; however, this example
    // does ensure a callback is provided.
    if (callback != nullptr) {
      callback.Call(context->Value(), {Number::New(env, *data)});
    }
  }
  if (data != nullptr) {
    // We're finished with the data.
    delete data;
  }
}

Napi::Object Init(Napi::Env env, Object exports) {
  exports.Set("start", Function::New(env, Start));
  return exports;
}

NODE_API_MODULE(clock, Init)
