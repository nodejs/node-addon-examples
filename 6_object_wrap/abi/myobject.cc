#include "myobject.h"

node::js::persistent constructor;

MyObject::MyObject(double value) : value_(value) {
}

MyObject::~MyObject() {
}

void MyObject::Init(node::js::value env, node::js::value exports) {
  node::js::value function = node::js::CreateConstructorForWrap(env, New);
  node::js::SetFunctionName(env, function, node::js::CreateString(env, "MyObject"));
  node::js::value prototype =
    node::js::GetProperty(env, function, node::js::PropertyName(env, "prototype"));

  node::js::SetProperty(env, prototype, node::js::PropertyName(env, "value"),
                        node::js::CreateFunction(env, GetValue));

  node::js::value plusOneFunction = node::js::CreateFunction(env, PlusOne);
  node::js::SetFunctionName(env, plusOneFunction, node::js::CreateString(env, "plusOne"));
  node::js::SetProperty(env, prototype, node::js::PropertyName(env, "plusOne"),
                        plusOneFunction);
  node::js::SetProperty(env, prototype, node::js::PropertyName(env, "multiply"),
                        node::js::CreateFunction(env, Multiply));

  constructor = node::js::CreatePersistent(env, function);

  node::js::SetProperty(env, exports, node::js::PropertyName(env, "MyObject"),
                        function);
}

void MyObject::New(node::js::value env, node::js::FunctionCallbackInfo info) {
/*
  if (info.IsConstructCall()) {
    // Invoked as constructor: `new MyObject(...)`
    double value = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
    MyObject* obj = new MyObject(value);
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = { info[0] };
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
*/

  // assumes its a constructor call
  if (node::js::IsContructCall(env, info)) {
    node::js::value args[1];
    node::js::GetCallbackArgs(info, args, 1);
    double value = 0;
    if (node::js::GetUndefined(env) != args[0]) {
      value = node::js::GetNumberFromValue(args[0]);
    }
    MyObject* obj = new MyObject(value);
    node::js::value jsobj = node::js::GetCallbackObject(env, info);
    node::js::Wrap(env, jsobj, (void*) obj, nullptr);
    node::js::SetReturnValue(env, info, jsobj);
  } else {
    node::js::value args[1];
    node::js::GetCallbackArgs(info, args, 1);
    const int argc = 1;
    node::js::value argv[argc] = { args[0] };
    node::js::value cons = node::js::GetPersistentValue(env, constructor);
    node::js::SetReturnValue(env, info, node::js::NewInstance(env, cons, argc, argv));
  }
}

void MyObject::GetValue(node::js::value env, node::js::FunctionCallbackInfo info) {
  MyObject* obj = (MyObject*) node::js::Unwrap(env, node::js::GetCallbackObject(env, info));
  node::js::SetReturnValue(env, info, node::js::CreateNumber(env, obj->value_));
}

void MyObject::PlusOne(node::js::value env, node::js::FunctionCallbackInfo info) {
  MyObject* obj = (MyObject*) node::js::Unwrap(env, node::js::GetCallbackObject(env, info));
  obj->value_ += 1;
  node::js::SetReturnValue(env, info, node::js::CreateNumber(env, obj->value_));
}

void MyObject::Multiply(node::js::value env, node::js::FunctionCallbackInfo info) {
  node::js::value args[1];
  node::js::GetCallbackArgs(info, args, 1);

  double multiple = 1;
  if (node::js::GetUndefined(env) != args[0]) {
    multiple = node::js::GetNumberFromValue(args[0]);
  }

  MyObject* obj = (MyObject*) node::js::Unwrap(env, node::js::GetCallbackObject(env, info));

  node::js::value cons = node::js::GetPersistentValue(env, constructor);
  const int argc = 1;
  node::js::value argv[argc] = { node::js::CreateNumber(env, obj->value_ * multiple) };
  node::js::SetReturnValue(env, info, node::js::NewInstance(env, cons, argc, argv));
}
