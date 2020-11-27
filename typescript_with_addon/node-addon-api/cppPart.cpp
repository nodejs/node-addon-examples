#include <iostream>
#include "cPart.h"

using namespace std;

int callingCPart(int value) {
  int result = cPart(++value);
  cout << "I'm a C++ function I'm sending " << value << " to C "
       << "and I received " << result << endl;
  return result;
}