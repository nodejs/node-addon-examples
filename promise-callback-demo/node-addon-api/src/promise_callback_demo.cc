#include <napi.h>
#include <future>
#include <iostream>
#include <thread>

using namespace Napi;

class PromiseCallbackDemo;
using Context = PromiseCallbackDemo;
using DataType = std::promise<std::string>;
void CallJs(Napi::Env env, Function callback, Context* context, DataType* data);
using TSFN = TypedThreadSafeFunction<Context, DataType, CallJs>;
using FinalizerDataType = void;

class PromiseCallbackDemo : public Napi::ObjectWrap<PromiseCallbackDemo> {
 public:
  PromiseCallbackDemo(const Napi::CallbackInfo&);

  static Napi::Function GetClass(Napi::Env);

  std::thread native_thread;
  TSFN tsfn;
};

void thread_entry(PromiseCallbackDemo* obj) {
  for (auto i = 0; i < 10; ++i) {
    std::promise<std::string> promise;
    auto future = promise.get_future();
    obj->tsfn.NonBlockingCall(&promise);

    // With exceptions disabled, STL calls will abort the thread, since we
    // cannot try/catch, as the calls to `std::make_exception_ptr()` will abort.
    try {
      auto result = future.get();
      std::cout << "Result from JS, call " << i << ": " << result << "\n";
    } catch (const std::exception& e) {
      std::cout << "Error from JS, call " << i << ": " << e.what() << "\n";
    }
  }
  obj->tsfn.Release();
}

void CallJs(Napi::Env env,
            Function callback,
            Context* context,
            DataType* data) {
  // Is the JavaScript environment still available to call into, eg. the TSFN is
  // not aborted
  if (env != nullptr) {
    auto jsPromise = callback.Call({}).As<Napi::Object>();
    auto then = jsPromise.Get("then").As<Function>();
    // Attach the resolve handler.
    // TODO proper error handling (add failure handler)
    then.Call(jsPromise,
              {Napi::Function::New(env,
                                   [=](const CallbackInfo& info) {
                                     auto result =
                                         info[0].As<String>().Utf8Value();
                                     data->set_value(result);
                                   }),
               Napi::Function::New(env, [=](const CallbackInfo& info) {
                 auto result = info[0].As<String>().Utf8Value();
                 data->set_exception(std::make_exception_ptr(
                     std::runtime_error("Error in jsCallback")));
               })});

  } else {
    data->set_exception(std::make_exception_ptr(
        std::runtime_error("Environment is shut down")));
  }
}

PromiseCallbackDemo::PromiseCallbackDemo(const Napi::CallbackInfo& info)
    : ObjectWrap(info) {
  Napi::Env env = info.Env();

  // Argument checking
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  if (!info[0].IsFunction()) {
    Napi::TypeError::New(env, "First argument must be a function")
        .ThrowAsJavaScriptException();
    return;
  }

  // Since we are storing the std::thread on this ObjectWrap instance, we should
  // reference it so it doesn't get garbage collected, and unreference it on
  // TSFN finalize.
  Ref();

  tsfn = TSFN::New(
      env,
      info[0].As<Function>(),  // JavaScript function called asynchronously
      "PromiseCallbackDemo",   // Name
      0,                       // Unlimited queue
      1,                       // Only one thread will use this initially
      this,                    // Context
      [](Napi::Env,
         FinalizerDataType*,
         Context* ctx) {  // Finalizer used to clean threads up
        ctx->Unref();
        ctx->native_thread.join();
      });

  native_thread = std::thread(thread_entry, this);
}

Napi::Function PromiseCallbackDemo::GetClass(Napi::Env env) {
  return DefineClass(env, "PromiseCallbackDemo", {});
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports["PromiseCallbackDemo"] = PromiseCallbackDemo::GetClass(env);
  return exports;
}

NODE_API_MODULE(addon, Init)
