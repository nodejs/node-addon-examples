#include <node.h>
#include "myobject.h"


MyObject::MyObject() {};
MyObject::~MyObject() {};

void MyObject::Destructor(void* nativeObject) {
  ((MyObject*) nativeObject)->~MyObject();
}

node::js::persistent MyObject::constructor;

void MyObject::Init(node::js::value env) {
  node::js::value function = node::js::CreateConstructorForWrap(env, New);
  node::js::SetFunctionName(env, function, node::js::CreateString(env, "MyObject"));

  constructor = node::js::CreatePersistent(env, function);
}

void MyObject::New(node::js::value env, node::js::FunctionCallbackInfo info) {
  node::js::value args[1];
  node::js::GetCallbackArgs(info, args, 1);
  MyObject* obj = new MyObject();
  obj->val_ = (args[0] == node::js::GetUndefined(env)) ? 0 : node::js::GetNumberFromValue(args[0]);
  node::js::value jsobj = node::js::GetCallbackObject(env, info);
  node::js::Wrap(env, jsobj, (void*) obj, MyObject::Destructor);
  node::js::SetReturnValue(env, info, jsobj);
}

node::js::value MyObject::NewInstance(node::js::value env, node::js::value arg) {
  const int argc = 1;
  node::js::value argv[argc] = { arg };
  node::js::value cons = node::js::GetPersistentValue(env, constructor);
  return node::js::NewInstance(env, cons, argc, argv);
}
