#ifndef MYOBJECT_NAPI_H
#define MYOBJECT_NAPI_H

#include <memory>
#include <napi.h>

#include "myobject.h"

class MyObjectNapi : public Napi::ObjectWrap<MyObjectNapi> {
  public:
    MyObjectNapi(const Napi::CallbackInfo& info);
    static void init(Napi::Env env, Napi::Object exports);
    Napi::Value multiply(const Napi::CallbackInfo& info);
    Napi::Value plusOne(const Napi::CallbackInfo& info);
    Napi::Value value(const Napi::CallbackInfo& info);
    
  private:
    std::shared_ptr<MyObject> _nativeMyObject;
};

#endif