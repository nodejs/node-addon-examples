#include <inttypes.h>
#include <napi.h>
#include <thread>

// A secondary thread increments a value starting from 0 and calls a JS callback
// on the JavaScript main thread.

// The ObjectWrap subclass whose instances own such a secondary thread.
class TsfnObjectWrap : public Napi::ObjectWrap<TsfnObjectWrap> {
 public:
  static Napi::Object Init(Napi::Env env) {
    return DefineClass(env, "TsfnObjectWrap", {});
  }

  TsfnObjectWrap(const Napi::CallbackInfo& info)
      : Napi::ObjectWrap<TsfnObjectWrap>(info) {
    // Whenever we construct a new instance of type TsfnObjectWrap, we construct
    // a thread-safe function that can be called from a secondary thread and
    // which provides a value to the JavaScript main thread.
    _tsfn = Napi::ThreadSafeFunction::New(
        info.Env(), info[0].As<Napi::Function>(), "TsfnObjectWrap", 1, 2);

    _thread = std::thread(&TsfnObjectWrap::Thread, std::ref(_tsfn));
  }

  ~TsfnObjectWrap() {
    _tsfn.Abort();
    _thread.join();
  }

 private:
  // This is the secondary thread.
  static void Thread(const Napi::ThreadSafeFunction& tsfn) {
    int64_t the_value = 0;
    int64_t buffer[3] = {0, 0, 0};
    int idx = 0;

    // Since we're calling the JavaScript main thread in a blocking fashion,
    // a buffer of three values is sufficient for synchronizing with the main
    // thread without losing a single value and without having to allocate an
    // integer on the heap for each call into JavaScript.
    while (true) {
      buffer[idx] = the_value;
      idx = (idx + 1) % 3;
      int64_t* value_ref = &buffer[idx];
      napi_status status = tsfn.BlockingCall(
          value_ref, [](Napi::Env env, Napi::Function fn, int64_t* data) {
            int64_t native_value = *data;
            Napi::Value result =
                fn.Call({Napi::Number::New(env, native_value)});
            if (result.IsEmpty()) {
              printf("main with %" PRId64 ": result was empty!\n",
                     native_value);
            } else {
              printf("main with %" PRId64 ": Done!\n",
                     result.As<Napi::Number>().Int64Value());
            }
          });
      // We break out of the infinite loop when we're informed that the thread-
      // safe function is being torn down.
      if (status == napi_closing) {
        break;
      }
      the_value++;
    }
  }

  Napi::ThreadSafeFunction _tsfn;
  std::thread _thread;
};

// Boilerplate code to define an add-on that consists of the above class.
Napi::Object TsfnObjectWrapExampleInit(Napi::Env env, Napi::Object exports) {
  return TsfnObjectWrap::Init(env);
}

NODE_API_MODULE(TsfnObjectWrapExample, TsfnObjectWrapExampleInit)
