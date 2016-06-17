```c++
#include <nan.h>

NAN_METHOD(RunCallback) {
  v8::Local<v8::Function> cb = info[0].As<v8::Function>();
  const unsigned argc = 1;
  v8::Local<v8::Value> argv[argc] = { Nan::New("hello world").ToLocalChecked() };
  Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, argc, argv);
}

void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  Nan::SetMethod(module, "exports", RunCallback);
}

NODE_MODULE(addon, Init)
```

As we see here, instead of setting a property on `module.exports`, it's also possible to replace `module.exports` property so you are exporting a single thing, the equivalent of: `module.exports = function () { ... }`.

To do that, the code here uses a longer form for our `Init` declaration.

The preprocessor expands `NAN_MODULE_INIT(Init)` to:


```c++
void Init(v8::Local<v8::Object> target)
```

To replace `exports`, we can declare a second argument, `module`, and set the `"exports"` property on that.

```c++
void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  Nan::SetMethod(module, "exports", RunCallback);
}
```

Later examples may also skip the `NAN_METHOD` macro and just use the long form:

```c++
void Method(const Nan::FunctionCallbackInfo<v8::Value>& info);
```

You should usually prefer to use the macros for maximum portability.