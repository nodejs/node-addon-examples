#include <napi.h>
#include <functional>
#include <memory>
#include <optional>
#include <thread>

using namespace Napi;

class ThreadSafeAsyncIteratorExample
    : public ObjectWrap<ThreadSafeAsyncIteratorExample> {
 public:
  ThreadSafeAsyncIteratorExample(const CallbackInfo& info)
      : ObjectWrap<ThreadSafeAsyncIteratorExample>(info),
        _current(info[0].As<Number>()),
        _last(info[1].As<Number>()) {}

  static Object Init(Napi::Env env, Napi::Object exports);

  Napi::Value Iterator(const CallbackInfo& info);

 private:
  using Context = ThreadSafeAsyncIteratorExample;

  struct DataType {
    std::unique_ptr<Promise::Deferred> deferred;
    bool done;
    std::optional<int> value;
  };

  static void CallJs(Napi::Env env,
                     Function callback,
                     Context* context,
                     DataType* data);

  using TSFN = TypedThreadSafeFunction<Context, DataType, CallJs>;

  using FinalizerDataType = void;

  int _current;
  int _last;
  TSFN _tsfn;
  std::thread _thread;
  std::unique_ptr<Promise::Deferred> _deferred;

  // Thread-safety
  std::mutex _mtx;
  std::condition_variable _cv;

  void threadEntry();

  static void FinalizerCallback(Napi::Env env,
                                void*,
                                ThreadSafeAsyncIteratorExample* context);
};

Object ThreadSafeAsyncIteratorExample::Init(Napi::Env env,
                                            Napi::Object exports) {
  Napi::Function func =
      DefineClass(env,
                  "ThreadSafeAsyncIteratorExample",
                  {InstanceMethod(Napi::Symbol::WellKnown(env, "asyncIterator"),
                                  &ThreadSafeAsyncIteratorExample::Iterator)});

  exports.Set("ThreadSafeAsyncIteratorExample", func);
  return exports;
}

Napi::Value ThreadSafeAsyncIteratorExample::Iterator(const CallbackInfo& info) {
  auto env = info.Env();

  if (_thread.joinable()) {
    Napi::Error::New(env, "Concurrent iterations not implemented.")
        .ThrowAsJavaScriptException();
    return Napi::Value();
  }

  _tsfn =
      TSFN::New(info.Env(),
                "tsfn",
                0,
                1,
                this,
                std::function<decltype(FinalizerCallback)>(FinalizerCallback));

  // To prevent premature garbage collection; Unref in TFSN finalizer
  Ref();

  // Create thread
  _thread = std::thread(&ThreadSafeAsyncIteratorExample::threadEntry, this);

  // Create iterable
  auto iterable = Napi::Object::New(env);

  iterable["next"] =
      Function::New(env, [this](const CallbackInfo& info) -> Napi::Value {
        std::lock_guard<std::mutex> lk(_mtx);
        auto env = info.Env();
        if (_deferred) {
          Napi::Error::New(env, "Concurrent iterations not implemented.")
              .ThrowAsJavaScriptException();
          return Napi::Value();
        }
        _deferred = std::make_unique<Promise::Deferred>(env);
        _cv.notify_all();
        return _deferred->Promise();
      });

  return iterable;
}

void ThreadSafeAsyncIteratorExample::threadEntry() {
  while (true) {
    std::unique_lock<std::mutex> lk(_mtx);
    _cv.wait(lk, [this] { return this->_deferred != nullptr; });
    auto done = _current > _last;
    if (done) {
      _tsfn.BlockingCall(new DataType{std::move(this->_deferred), true, {}});
      break;
    } else {
      std::this_thread::sleep_for(
          std::chrono::seconds(1));  // Simulate CPU-intensive work
      _tsfn.BlockingCall(
          new DataType{std::move(this->_deferred), false, _current++});
    }
  }
  _tsfn.Release();
}

void ThreadSafeAsyncIteratorExample::CallJs(Napi::Env env,
                                            Function callback,
                                            Context* context,
                                            DataType* data) {
  if (env != nullptr) {
    auto value = Object::New(env);

    if (data->done) {
      value["done"] = Boolean::New(env, true);
    } else {
      value["done"] = Boolean::New(env, false);
      value["value"] = Number::New(env, data->value.value());
    }
    data->deferred->Resolve(value);
  }

  if (data != nullptr) {
    delete data;
  }
}

void ThreadSafeAsyncIteratorExample::FinalizerCallback(
    Napi::Env env, void*, ThreadSafeAsyncIteratorExample* context) {
  context->_thread.join();
  context->Unref();
}

Napi::Object Init(Napi::Env env, Object exports) {
  ThreadSafeAsyncIteratorExample::Init(env, exports);
  return exports;
}

NODE_API_MODULE(example, Init)
