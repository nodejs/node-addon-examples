
// based on:
// https://github.com/fcanas/node-native-boilerplate
// https://github.com/nodejs/nan/blob/master/doc/object_wrappers.md

#include <nan.h>

#include "myobject.hpp"

// module initialization

NAN_MODULE_INIT(init)
{
    // passing target down to the next NAN_MODULE_INIT
    MyObject::init(target);
}

NODE_MODULE(addon, init)
