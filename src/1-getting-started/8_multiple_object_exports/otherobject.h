#ifndef OTHEROBJECT_H
#define OTHEROBJECT_H

#include <napi.h>
#include <string>

class OtherObject {
  public:
    OtherObject(std::string value);
    std::string multiply(int value);
    std::string append(std::string value);
    std::string value();
    void value(std::string value);
    
  private:
    std::string _value;
};

#endif