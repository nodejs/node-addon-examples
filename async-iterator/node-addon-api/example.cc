#include <napi.h>
#include <memory>

using namespace Napi;

class AsyncIteratorExample : public ObjectWrap<AsyncIteratorExample> {
 public:
  AsyncIteratorExample(const CallbackInfo& info)
      : ObjectWrap<AsyncIteratorExample>(info),
        _from(info[0].As<Number>()),
        _to(info[1].As<Number>()) {}

  static Object Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(
        env,
        "AsyncIteratorExample",
        {InstanceMethod(Napi::Symbol::WellKnown(env, "asyncIterator"),
                        &AsyncIteratorExample::Iterator)});

    exports.Set("AsyncIteratorExample", func);
    return exports;
  }

  Napi::Value Iterator(const CallbackInfo& info) {
    auto env = info.Env();
    auto iteratorObject = Napi::Object::New(env);

    iteratorObject["current"] = Number::New(env, _from);
    iteratorObject["last"] = Number::New(env, _to);
    auto next = Function::New(env, [](const CallbackInfo& info) {
      auto env = info.Env();
      auto deferred =
          std::make_shared<Promise::Deferred>(Promise::Deferred::New(env));
      auto iteratorObject = info.This().As<Object>();
      auto callback = Function::New(
          env,
          [=](const CallbackInfo& info) {
            auto env = info.Env();
            auto value = Object::New(env);
            auto iteratorObject = info.This().As<Object>();

            auto current =
                iteratorObject.Get("current").As<Number>().Int32Value();
            auto last = iteratorObject.Get("last").As<Number>().Int32Value();
            auto done = current > last;

            if (done) {
              value["done"] = Boolean::New(env, true);
            } else {
              value["done"] = Boolean::New(env, false);
              value["value"] = Number::New(env, current);
              iteratorObject["current"] = Number::New(env, current + 1);
            }
            deferred->Resolve(value);
          },
          "next");

      env.Global()
          .Get("setTimeout")
          .As<Function>()
          .Call({callback.Get("bind").As<Function>().Call(callback,
                                                          {iteratorObject}),
                 Number::New(env, 1000)});

      return deferred->Promise();
    });

    iteratorObject["next"] =
        next.Get("bind").As<Function>().Call(next, {iteratorObject});

    return iteratorObject;
  }

 private:
  int _from;
  int _to;
};

Napi::Object Init(Napi::Env env, Object exports) {
  AsyncIteratorExample::Init(env, exports);
  return exports;
}

NODE_API_MODULE(example, Init)
