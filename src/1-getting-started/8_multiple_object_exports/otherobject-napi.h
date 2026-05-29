#ifndef OTHEROBJECT_NAPI_H
#define OTHEROBJECT_NAPI_H

#include <memory>
#include <napi.h>

#include "otherobject.h"

class OtherObjectNapi : public Napi::ObjectWrap<OtherObjectNapi> {
  public:
    OtherObjectNapi(const Napi::CallbackInfo& info);
    static void init(Napi::Env env, Napi::Object exports);
    Napi::Value multiply(const Napi::CallbackInfo& info);
    Napi::Value append(const Napi::CallbackInfo& info);
    Napi::Value value(const Napi::CallbackInfo& info);
    
  private:
    std::shared_ptr<OtherObject> _nativeOtherObject;
};

#endif