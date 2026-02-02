#include <napi.h>

static Napi::String Method(const Napi::CallbackInfo& info) {
  // Napi::Env is the opaque data structure containing the environment in which
  // the request is being run. We will need this env when we want to create any
  // new objects inside of the node.js environment
  Napi::Env env = info.Env();

  // Create a C++ level variable
  std::string helloWorld = "Hello, world!";

  // Return a new javascript string that we copy-construct inside of the node.js
  // environment
  return Napi::String::New(env, helloWorld);
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "hello"),
              Napi::Function::New(env, Method));
  return exports;
}

NODE_API_MODULE(hello, Init)
