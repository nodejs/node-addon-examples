#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <napi.h>

class MyObject {
  public:
    MyObject(double value);
    double multiply(double value);
    double plusOne();
    double value();
    void value(double value);
    
  private:
    double _value;
};

#endif