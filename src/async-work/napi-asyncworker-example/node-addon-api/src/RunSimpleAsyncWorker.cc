#include "SimpleAsyncWorker.h"

Value runSimpleAsyncWorker(const CallbackInfo& info) {
  int runTime = info[0].As<Number>();
  Function callback = info[1].As<Function>();
  SimpleAsyncWorker* asyncWorker = new SimpleAsyncWorker(callback, runTime);
  asyncWorker->Queue();
  std::string msg =
      "SimpleAsyncWorker for " + std::to_string(runTime) + " seconds queued.";
  return String::New(info.Env(), msg.c_str());
};

Object Init(Env env, Object exports) {
  exports["runSimpleAsyncWorker"] = Function::New(
      env, runSimpleAsyncWorker, std::string("runSimpleAsyncWorker"));
  return exports;
}

NODE_API_MODULE(addon, Init)