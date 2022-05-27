#include <napi.h>
#include <cstdio>

static Napi::Value AcceptByteArray(const Napi::CallbackInfo& info) {
  if (info.Length() != 1) {
    Napi::Error::New(info.Env(), "Expected exactly one argument")
        .ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }

  if (!info[0].IsTypedArray()) {
    Napi::Error::New(info.Env(), "Expected a TypedArray")
        .ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }
  Napi::TypedArray typedArray = info[0].As<Napi::TypedArray>();

  if (typedArray.TypedArrayType() != napi_uint8_array) {
    Napi::Error::New(info.Env(), "Expected an Uint8Array")
        .ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }
  Napi::Uint8Array uint8Array = typedArray.As<Napi::Uint8Array>();

  // Copy to std::vector<uint8_t>:
  std::vector<uint8_t> bytes(uint8Array.Data(),
                             uint8Array.Data() + uint8Array.ElementLength());
  printf("std::vector<uint8_t> from Uint8Array: [");
  for (uint8_t byte : bytes) {
    printf("%d, ", byte);
  }
  printf("\b\b]\n");

  return info.Env().Undefined();
}

static Napi::Value CreateByteArray(const Napi::CallbackInfo& info) {
  if (info.Length() != 1) {
    Napi::Error::New(info.Env(), "Expected exactly one argument")
        .ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }

  if (!info[0].IsArray()) {
    Napi::Error::New(info.Env(), "Expected an Array")
        .ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }
  Napi::Array array = info[0].As<Napi::Array>();
  size_t arrayLength = array.Length();

  // Create std::vector<uint8_t> out of array.
  // We allocate it on the heap to allow wrapping it up into ArrayBuffer.
  std::unique_ptr<std::vector<uint8_t>> nativeArray =
      std::make_unique<std::vector<uint8_t>>(arrayLength, 0);
  for (size_t i = 0; i < arrayLength; ++i) {
    Napi::Value arrayItem = array[i];
    if (!arrayItem.IsNumber()) {
      Napi::Error::New(info.Env(), "Expected a Number as Array Item")
          .ThrowAsJavaScriptException();
      return info.Env().Undefined();
    }
    Napi::Number number = arrayItem.As<Napi::Number>();
    double numberValue = number.DoubleValue();
    if (numberValue < 0 || numberValue > 255) {
      Napi::Error::New(info.Env(),
                       "Array Item Number value is out of range [0..255]")
          .ThrowAsJavaScriptException();
      return info.Env().Undefined();
    }

    (*nativeArray)[i] = static_cast<uint8_t>(numberValue);
  }

  printf("std::vector<uint8_t> from Array: [");
  for (uint8_t byte : *nativeArray) {
    printf("%d, ", byte);
  }
  printf("\b\b]\n");

  // Wrap up the std::vector into the ArrayBuffer.
  // Note: instead of wrapping the std::vector we could allow ArrayBuffer to
  // create internal storage that copies the std::vector, but it is less
  // efficient because it requires an extra memory allocation.
  Napi::ArrayBuffer arrayBuffer = Napi::ArrayBuffer::New(
      info.Env(),
      nativeArray->data(),
      arrayLength /* size in bytes */,
      [](Napi::Env /*env*/, void* /*data*/, std::vector<uint8_t>* hint) {
        std::unique_ptr<std::vector<uint8_t>> vectorPtrToDelete(hint);
      },
      nativeArray.get());
  // The finalizer is responsible for deleting the vector: release the
  // unique_ptr ownership.
  nativeArray.release();

  Napi::Uint8Array byteArray = Napi::Uint8Array::New(info.Env(), arrayLength, arrayBuffer, 0);
  
  return byteArray;
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports["AcceptByteArray"] = Napi::Function::New(env, AcceptByteArray);
  exports["CreateByteArray"] = Napi::Function::New(env, CreateByteArray);
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
