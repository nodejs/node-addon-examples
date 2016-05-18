#ifndef TIMER_H_
#define TIMER_H_

#include <nan.h>
    
class Timer : public Nan::ObjectWrap {
 public:
  Nan::Callback* emit = nullptr;
  static void Init(v8::Local<v8::Object> exports);
  static void onTick(uv_timer_t *handle);
 private:
  Timer();
  ~Timer();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Start(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Stop(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static Nan::Persistent<v8::Function> constructor;
  uv_timer_t *timer_req_ = nullptr;
};


#endif  // TIMER_H_
