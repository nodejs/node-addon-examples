#include <math.h>
#include <node.h>

using namespace v8;

// Class containing information needed for as long as the addon exists. It
// replaces the use of global static data with per-addon-instance data by
// associating an instance of this class with each instance of this addon during
// addon initialization. The instance of this class is then passed to each
// binding the addon provides. Thus, the data stored in an instance of this
// class is available to each binding, just as global static data would be.
class AddonData {
 public:
  // This is the actual, useful work performed: increment or decrement the value
  // stored per addon instance after passing it through a CPU-consuming but
  // otherwise useless calculation. Round the result to the nearest integer.
  int GetNewValue(double offset) {
    // Expensively increment or decrement the value.
    value = tan(atan(exp(log(sqrt(value * value))))) + offset;

    // Round the value to the nearest integer.
    value =
        (double)(((int)value) + (value - ((double)(int)value) > 0.5 ? 1 : 0));

    // Return the value as an integer.
    return (int)value;
  }

  // This is boilerplate. It creates a new instance of this class and wraps it
  // into a v8::External. The isolate and the exports are necessary, because we
  // want the instance of this class to be destroyed along with the exports
  // object when the addon is eventually unloaded.
  static Local<Value> New(Isolate* isolate, Local<Object> exports) {
    return External::New(isolate, new AddonData(isolate, exports));
  }

 private:
  // This is the actual, useful payload carried by an instance of this class.
  // A double value is kept around for as long as an instance of this addon is
  // loaded, and is incremented or decremented whenever the addon receives a
  // call from JavaScript.
  double value;

  explicit AddonData(Isolate* isolate, Local<Object> exports)
      :  // The payload is initialized here. The rest of the constructor is
         // boilerplate that ensures that the instance of this addon data is
         // destroyed along with the instance of this addon (`exports`).
        value(0.0) {
    exports_persistent.Reset(isolate, exports);
    exports_persistent.SetWeak(this, DeleteMe, WeakCallbackType::kParameter);
  }

  // The rest of the class definition is boilerplate.

  // The persistent reference must be reset before the instance of this class is
  // destroyed, otherwise memory will leak on the V8 side.
  ~AddonData() { exports_persistent.Reset(); }

  // This static function will be called when the addon instance is unloaded. It
  // merely destroys the per-addon-instance data.
  static void DeleteMe(const WeakCallbackInfo<AddonData>& info) {
    delete info.GetParameter();
  }

  Persistent<Object> exports_persistent;
};

// Function called from JavaScript to increment the value stored in this addon.
void Increment(const FunctionCallbackInfo<Value>& info) {
  // Retrieve the per-addon-instance data.
  AddonData* addon_data =
      static_cast<AddonData*>(info.Data().As<External>()->Value());

  info.GetReturnValue().Set(
      Number::New(info.GetIsolate(), addon_data->GetNewValue(1.0)));
}

// Function called from JavaScript to decrement the value stored in this addon.
void Decrement(const FunctionCallbackInfo<Value>& info) {
  // Retrieve the per-addon-instance data.
  AddonData* addon_data =
      static_cast<AddonData*>(info.Data().As<External>()->Value());

  info.GetReturnValue().Set(
      Number::New(info.GetIsolate(), addon_data->GetNewValue(-1.0)));
}

// Initialize the addon in such a way that it may be initialized multiple times
// per process. The function body following this macro is provided the value
// `exports` of type `Local<Object>`, the value `module` of type
// `Local<Object>`, and `context` of type `Local<Context>`. It may either define
// new properties on the `exports` object, or define the property named
// "exports" on the `module` object.
NODE_MODULE_INIT(/*exports, module, context*/) {
  Isolate* isolate = context->GetIsolate();

  // Create a new instance of the addon data that will be associated with this
  // instance of the addon, and that will be freed along with this instance of
  // the addon.
  Local<Value> addon_data = AddonData::New(isolate, exports);

  // Export the functions we wish to make available to JavaScript.

  exports
      ->Set(context,
            String::NewFromUtf8(isolate, "increment", NewStringType::kNormal)
                .ToLocalChecked(),
            FunctionTemplate::New(isolate, Increment, addon_data)
                ->GetFunction(context)
                .ToLocalChecked())
      .FromJust();

  exports
      ->Set(context,
            String::NewFromUtf8(isolate, "decrement", NewStringType::kNormal)
                .ToLocalChecked(),
            FunctionTemplate::New(isolate, Decrement, addon_data)
                ->GetFunction(context)
                .ToLocalChecked())
      .FromJust();
}
