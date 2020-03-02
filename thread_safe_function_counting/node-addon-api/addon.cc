#include "napi.h"
#include <chrono>
#include <thread>

constexpr size_t ARRAY_LENGTH = 10;

// Data structure representing our thread-safe function context.
struct TsfnContext {

  TsfnContext(Napi::Env env) : deferred(Napi::Promise::Deferred::New(env)) {
    for (size_t i = 0; i < ARRAY_LENGTH; ++i)
      ints[i] = i;
  };

  // Native Promise returned to JavaScript
  Napi::Promise::Deferred deferred;

  // Native thread
  std::thread nativeThread;

  // Some data to pass around
  int ints[ARRAY_LENGTH];

  Napi::ThreadSafeFunction tsfn;
};

// The thread entry point. This takes as its arguments the specific
// threadsafe-function context created inside the main thread.
void threadEntry(TsfnContext *context);

// The thread-safe function finalizer callback. This callback executes
// at destruction of thread-safe function, taking as arguments the finalizer
// data and threadsafe-function context.
void FinalizerCallback(Napi::Env env, void *finalizeData, TsfnContext *context);

// Exported JavaScript function. Creates the thread-safe function and native
// thread. Promise is resolved in the thread-safe function's finalizer.
Napi::Value CreateTSFN(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  // Construct context data
  auto testData = new TsfnContext(env);

  // Create a new ThreadSafeFunction.
  testData->tsfn =
      Napi::ThreadSafeFunction::New(env,                    // Environment
                              info[0].As<Napi::Function>(), // JS function from caller
                              "TSFN",                 // Resource name
                              0,        // Max queue size (0 = unlimited).
                              1,        // Initial thread count
                              testData, // Context,
                              FinalizerCallback, // Finalizer
                              (void *)nullptr    // Finalizer data
      );
  testData->nativeThread = std::thread(threadEntry, testData);

  // Return the deferred's Promise. This Promise is resolved in the thread-safe
  // function's finalizer callback.
  return testData->deferred.Promise();
}

// The thread entry point. This takes as its arguments the specific
// threadsafe-function context created inside the main thread.
void threadEntry(TsfnContext *context) {

  // This callback transforms the native addon data (int *data) to JavaScript
  // values. It also receives the treadsafe-function's registered callback, and
  // may choose to call it.
  auto callback = [](Napi::Env env, Napi::Function jsCallback, int *data) {
    jsCallback.Call({Napi::Number::New(env, *data)});
  };

  for (size_t index = 0; index < ARRAY_LENGTH; ++index) {
    // Perform a call into JavaScript.
    napi_status status =
        context->tsfn.BlockingCall(&context->ints[index], callback);

    if (status != napi_ok) {
      Napi::Error::Fatal("ThreadEntry", "Napi::ThreadSafeNapi::Function.BlockingCall() failed");
    }
    // Sleep for some time.
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }

  // Release the thread-safe function. This decrements the internal thread
  // count, and will perform finalization since the count will reach 0.
  context->tsfn.Release();
}

void FinalizerCallback(Napi::Env env, void *finalizeData,
                       TsfnContext *context) {
  // Join the thread
  context->nativeThread.join();

  // Resolve the Promise previously returned to JS via the CreateTSFN method.
  context->deferred.Resolve(Napi::Boolean::New(env, true));
  delete context;
}

// Addon entry point
Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports["createTSFN"] = Napi::Function::New(env, CreateTSFN);
  return exports;
}

NODE_API_MODULE(addon, Init)
