#include <napi.h>

#include <chrono>
#include <thread>

class Dispatcher : public Napi::AsyncWorker {
 public:
  Dispatcher(Napi::Env env, Napi::Function& callback)
      : Napi::AsyncWorker(env),
        callback_(callback),
        tsfn_{
            Napi::ThreadSafeFunction::New(env, callback, "Dispatcher", 0, 1)} {}

  virtual ~Dispatcher() override { tsfn_.Release(); }

  void Execute() override {
    // Since this method executes on a thread that is different from the main
    // thread, we can't directly call into JavaScript. To trigger a call into
    // JavaScript from this thread, ThreadSafeFunction needs to be used to
    // communicate with the main thread, so that the main thread can invoke the
    // JavaScript function.

    napi_status status =
        tsfn_.BlockingCall([](Napi::Env env, Napi::Function js_callback) {
          js_callback.Call({Napi::String::New(env, "open")});
        });
    if (status != napi_ok) {
      SetError("Napi::ThreadSafeNapi::Function.BlockingCall() failed");
      return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    status = tsfn_.BlockingCall([](Napi::Env env, Napi::Function js_callback) {
      js_callback.Call(
          {Napi::String::New(env, "message"), Napi::String::New(env, "data1")});
    });
    if (status != napi_ok) {
      SetError("Napi::ThreadSafeNapi::Function.BlockingCall() failed");
      return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    status = tsfn_.BlockingCall([](Napi::Env env, Napi::Function js_callback) {
      js_callback.Call(
          {Napi::String::New(env, "message"), Napi::String::New(env, "data2")});
    });
    if (status != napi_ok) {
      SetError("Napi::ThreadSafeNapi::Function.BlockingCall() failed");
      return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    status = tsfn_.BlockingCall([](Napi::Env env, Napi::Function js_callback) {
      js_callback.Call(
          {Napi::String::New(env, "message"), Napi::String::New(env, "data3")});
    });
    if (status != napi_ok) {
      SetError("Napi::ThreadSafeNapi::Function.BlockingCall() failed");
      return;
    }

    status = tsfn_.BlockingCall([](Napi::Env env, Napi::Function js_callback) {
      js_callback.Call({Napi::String::New(env, "close")});
    });
    if (status != napi_ok) {
      SetError("Napi::ThreadSafeNapi::Function.BlockingCall() failed");
      return;
    }
  }

  void OnError(Napi::Error const& error) override {
    callback_.Call({Napi::String::New(Env(), "error"), error.Value()});
  }

 private:
  Napi::Function callback_;
  Napi::ThreadSafeFunction tsfn_;
};

void Dispatch(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1 || !info[0].IsFunction()) {
    Napi::TypeError::New(
        env, "The first argument needs to be the callback function.")
        .ThrowAsJavaScriptException();
    return;
  }

  Napi::Function callback = info[0].As<Napi::Function>();

  Dispatcher* dispatcher = new Dispatcher(env, callback);
  dispatcher->Queue();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("dispatch", Napi::Function::New(env, Dispatch));
  return exports;
}

NODE_API_MODULE(dispatcher, Init)
