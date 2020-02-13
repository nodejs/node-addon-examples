#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <node_api.h>

#define CHECK(expr) \
  { \
    if ((expr) == 0) { \
      fprintf(stderr, "%s:%d: failed assertion `%s'\n", __FILE__, __LINE__, #expr); \
      fflush(stderr); \
      abort(); \
    } \
  }

// Structure containing information needed for as long as the addon exists. It
// replaces the use of global static data with per-addon-instance data by
// associating an instance of this structure with each instance of this addon
// during addon initialization. The instance of this structure is then passed to
// each binding the addon provides. Thus, the data stored in an instance of this
// structure is available to each binding, just as global static data would be.
typedef struct {
  double value;
} AddonData;

// This is the actual, useful work performed: increment or decrement the value
// stored per addon instance after passing it through a CPU-consuming but
// otherwise useless calculation.
static int ModifyAddonData(AddonData* data, double offset) {
    // Expensively increment or decrement the value.
    data->value = tan(atan(exp(log(sqrt(data->value * data->value))))) + offset;

    // Round the value to the nearest integer.
    data->value =
        (double)(((int)data->value) +
        (data->value - ((double)(int)data->value) > 0.5 ? 1 : 0));

    // Return the value as an integer.
    return (int)(data->value);
}

// This is boilerplate. The instance of the `AddonData` structure created during
// addon initialization must be destroyed when the addon is unloaded. This
// function will be called when the addon's `exports` object is garbage collected.
static void DeleteAddonData(napi_env env, void* data, void* hint) {
  // Avoid unused parameter warnings.
  (void) env;
  (void) hint;

  // Free the per-addon-instance data.
  free(data);
}

// This is also boilerplate. It creates and initializes an instance of the
// `AddonData` structure and ties its lifecycle to that of the addon instance's
// `exports` object. This means that the data will be available to this instance
// of the addon for as long as the JavaScript engine keeps it alive.
static AddonData* CreateAddonData(napi_env env, napi_value exports) {
  AddonData* result = malloc(sizeof(*result));
  result->value = 0.0;
  CHECK(napi_wrap(env,
                   exports,
                   result,
                   DeleteAddonData,
                   NULL,
                   NULL) == napi_ok);
  return result;
}

// This function is called from JavaScript. It uses an expensive operation to
// increment the value stored inside the `AddonData` structure by one.
static napi_value Increment(napi_env env, napi_callback_info info) {
  // Retrieve the per-addon-instance data.
  AddonData* addon_data = NULL;
  CHECK(napi_get_cb_info(env,
                          info,
                          NULL,
                          NULL,
                          NULL,
                          ((void**)&addon_data)) == napi_ok);

  // Increment the per-addon-instance value and create a new JavaScript integer
  // from it.
  napi_value result;
  CHECK(napi_create_int32(env,
                           ModifyAddonData(addon_data, 1.0),
                           &result) == napi_ok);

  // Return the JavaScript integer back to JavaScript.
  return result;
}

// This function is called from JavaScript. It uses an expensive operation to
// decrement the value stored inside the `AddonData` structure by one.
static napi_value Decrement(napi_env env, napi_callback_info info) {
  // Retrieve the per-addon-instance data.
  AddonData* addon_data = NULL;
  CHECK(napi_get_cb_info(env,
                          info,
                          NULL,
                          NULL,
                          NULL,
                          ((void**)&addon_data)) == napi_ok);

  // Decrement the per-addon-instance value and create a new JavaScript integer
  // from it.
  napi_value result;
  CHECK(napi_create_int32(env,
                           ModifyAddonData(addon_data, -1.0),
                           &result) == napi_ok);

  // Return the JavaScript integer back to JavaScript.
  return result;
}

// Initialize the addon in such a way that it may be initialized multiple times
// per process. The function body following this macro is provided the value
// `env` which has type `napi_env` and the value `exports` which has type
// `napi_value` and which refers to a JavaScript object that ultimately contains
// the functions this addon wishes to expose. At the end, it must return a
// `napi_value`. It may return `exports`, or it may create a new `napi_value`
// and return that instead.
NAPI_MODULE_INIT(/*env, exports*/) {
  // Create a new instance of the per-instance-data that will be associated with
  // the instance of the addon being initialized here and that will be destroyed
  // along with the instance of the addon.
  AddonData* addon_data = CreateAddonData(env, exports);

  // Declare the bindings this addon provides. The data created above is given
  // as the last initializer parameter, and will be given to the binding when it
  // is called.
  napi_property_descriptor bindings[] = {
    {"increment", NULL, Increment, NULL, NULL, NULL, napi_enumerable, addon_data},
    {"decrement", NULL, Decrement, NULL, NULL, NULL, napi_enumerable, addon_data}
  };

  // Expose the two bindings declared above to JavaScript.
  CHECK(napi_define_properties(env,
                                exports,
                                sizeof(bindings) / sizeof(bindings[0]),
                                bindings) == napi_ok);

  // Return the `exports` object provided. It now has two new properties, which
  // are the functions we wish to expose to JavaScript.
  return exports;
}
