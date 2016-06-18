#include <node.h>
#include <iostream>
#include "timer.h"

Timer::Timer() {};
Timer::~Timer() {
  if (emit != nullptr) {
    delete emit;
  }
};

Nan::Persistent<v8::Function> Timer::constructor;
void Timer::Init(v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Timer").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "start", Start);
  Nan::SetPrototypeMethod(tpl, "stop", Stop);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("Timer").ToLocalChecked(), tpl->GetFunction());
}

void Timer::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (!info.IsConstructCall()) {
    return Nan::ThrowError("`new` required");
  }
  Timer* obj = new Timer();
  obj->Wrap(info.This());
  //  "access" emit function inherited from EventEmitter
  obj->emit = new Nan::Callback(
    v8::Local<v8::Function>::Cast(obj->handle()->Get(Nan::New("emit").ToLocalChecked())));
  info.GetReturnValue().Set(info.This());
}

void Timer::Start(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Timer* obj = Nan::ObjectWrap::Unwrap<Timer>(info.Holder());
  if (obj->timer_req_ != nullptr) {
    // already started
    return;
  }
  uint64_t timeout = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
  uint64_t repeat = info[1]->IsUndefined() ? 0 : info[1]->NumberValue();
  uv_timer_t *tr = new uv_timer_t;
  obj->timer_req_ = tr;
  tr->data = obj;
  uv_timer_init(uv_default_loop(), tr);
  uv_timer_start(tr, onTick, timeout, repeat);
}

void Timer::Stop(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Timer* obj = Nan::ObjectWrap::Unwrap<Timer>(info.Holder());
  if (obj->timer_req_ == nullptr) {
    return;
  }
  uv_timer_stop(obj->timer_req_);
  delete obj->timer_req_;
  obj->timer_req_ = nullptr;
}

void Timer::onTick(uv_timer_t *handle) {
  Nan::HandleScope scope;
  Timer *obj = static_cast<Timer*>(handle->data);
  v8::Local<v8::Value> argv[] = { Nan::New("tick").ToLocalChecked() };
  obj->emit->Call(obj->handle(), 1, argv);
}
