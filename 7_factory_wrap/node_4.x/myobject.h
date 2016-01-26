#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <node.h>
#include <node_object_wrap.h>

namespace demo {

using v8::Function;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Persistent;
using v8::Value;

class MyObject : public node::ObjectWrap {
 public:
  static void Init(Isolate* isolate);
  static void NewInstance(const FunctionCallbackInfo<Value>& args);

 private:
  explicit MyObject(double value = 0);
  ~MyObject();

  static void New(const FunctionCallbackInfo<Value>& args);
  static void PlusOne(const FunctionCallbackInfo<Value>& args);
  static Persistent<Function> constructor;
  double value_;
};

} // namespace demo

#endif
