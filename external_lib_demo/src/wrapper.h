#pragma once

#include <napi.h>

class ExternalLib : public Napi::ObjectWrap<ExternalLib>
{
public:
    ExternalLib(const Napi::CallbackInfo&);
    Napi::Value Greet(const Napi::CallbackInfo&);

    static Napi::Function GetClass(Napi::Env);

private:
    std::string _greeterName;
};
