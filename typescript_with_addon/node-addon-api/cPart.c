#include <stdio.h>
#include "cPart.h"

int cPart(int value) {
  int newValue = value + 30;
  printf("I'm a C function and I received %d from C++ and I'm sending back %d \n",value,newValue);
  return newValue;
}