
#pragma once

#include <nan.h>

class MyObject : public Nan::ObjectWrap
{
private:

    static Nan::Persistent<v8::Function> constructor;
    double val_;

    MyObject();
    ~MyObject();

    static NAN_METHOD(New);

public:

    double val() const;

    static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> arg);
    static NAN_MODULE_INIT(init);
};
