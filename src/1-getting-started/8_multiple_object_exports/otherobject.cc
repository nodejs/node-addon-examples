#include "otherobject.h"

OtherObject::OtherObject(std::string value) {
  _value = value;
}

std::string OtherObject::multiply(int value) {
  std::string original = _value;
  
  for ( int i = 0; i < value; i++ )
    _value += original;
    
  return _value;
}

std::string OtherObject::append(std::string value) {
  _value += value;
  
  return _value;
}

std::string OtherObject::value() {
  return _value;
}

void OtherObject::value(std::string value) {
  _value = value;
}
