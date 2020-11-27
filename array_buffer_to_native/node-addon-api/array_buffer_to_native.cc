#include <napi.h>
#include <stdio.h>

static void ArrayConsumer(const int32_t* array, size_t length) {
  for (size_t index = 0; index < length; index++) {
    fprintf(stderr, "array[%lu] = %d\n", index, array[index]);
  }
}

static Napi::Value AcceptArrayBuffer(const Napi::CallbackInfo& info) {
  if (info.Length() != 1) {
    Napi::Error::New(info.Env(), "Expected exactly one argument")
        .ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }
  if (!info[0].IsArrayBuffer()) {
    Napi::Error::New(info.Env(), "Expected an ArrayBuffer")
        .ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }

  Napi::ArrayBuffer buf = info[0].As<Napi::ArrayBuffer>();

  ArrayConsumer(reinterpret_cast<int32_t*>(buf.Data()),
                buf.ByteLength() / sizeof(int32_t));

  return info.Env().Undefined();
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports["AcceptArrayBuffer"] = Napi::Function::New(env, AcceptArrayBuffer);
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
