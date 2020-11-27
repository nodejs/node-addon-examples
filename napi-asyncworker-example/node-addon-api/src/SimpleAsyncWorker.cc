#include "SimpleAsyncWorker.h"
#include <chrono>
#include <thread>

SimpleAsyncWorker::SimpleAsyncWorker(Function& callback, int runTime)
    : AsyncWorker(callback), runTime(runTime){};

void SimpleAsyncWorker::Execute() {
  std::this_thread::sleep_for(std::chrono::seconds(runTime));
  if (runTime == 4) SetError("Oops! Failed after 'working' 4 seconds.");
};

void SimpleAsyncWorker::OnOK() {
  std::string msg = "SimpleAsyncWorker returning after 'working' " +
                    std::to_string(runTime) + " seconds.";
  Callback().Call({Env().Null(), String::New(Env(), msg)});
};