
#pragma once

#include <nan.h>

class MyObject : public Nan::ObjectWrap
{
private:

    static Nan::Persistent<v8::Function> constructor;
    double value_;

    explicit MyObject(double value = 0);
    ~MyObject();

    static NAN_METHOD(New);

    static NAN_METHOD(getValue);
    static NAN_METHOD(plusOne);
    static NAN_METHOD(multiply);  ///< @return a new object whose `value_` is `this->value_ * number`

public:

    static NAN_MODULE_INIT(init);
};
