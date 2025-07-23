#include "myobject.h"

MyObject::MyObject(double value) {
  _value = value;
}

double MyObject::multiply(double value) {
  _value *= value;
  
  return _value;
}

double MyObject::plusOne() {
  _value++;
  
  return _value;
}

double MyObject::value() {
  return _value;
}

void MyObject::value(double value) {
  _value = value;
}
