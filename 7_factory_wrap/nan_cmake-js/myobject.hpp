
#pragma once

#include <nan.h>

class MyObject : public Nan::ObjectWrap
{
private:

    static Nan::Persistent<v8::Function> constructor;
    double counter_;

    MyObject();
    ~MyObject();

    static NAN_METHOD(New);
    static NAN_METHOD(plusOne);

 public:

    static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> arg);
    static NAN_MODULE_INIT(init);
};
