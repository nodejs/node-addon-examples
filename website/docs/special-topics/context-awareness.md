---
id: special-topics.context-awareness
title: Context awareness
prev: special-topics.thread-safe-functions
---

## Background

Node.js has historically run as a single-threaded process. This all changed with the introduction of [Worker Threads](https://nodejs.org/api/worker_threads.html#worker_threads_worker_threads) in Node 10. Worker Threads add a JavaScript-friendly concurrency abstraction that native add-on developers need to be aware of. What this means practically is that your native add-on may be loaded and unloaded more than once and its code may be executed concurrently in multiple threads. There are specific steps you must take to insure your native add-on code runs correctly.

The Worker Thread model specifies that each Worker runs completely independently of each other and communicate to the parent Worker using a MessagePort object supplied by the parent. This makes the Worker Threads essentially isolated from one another. The same is true for your native add-on. 

Each Worker Thread operates within its own environment which is also referred to as a context. The context is available to each Node-API function as an [`napi_env`](https://nodejs.org/api/n-api.html#n_api_napi_env) value.

## Multiple loading and unloading

If your native add-on requires persistent memory, allocating this memory in static global space is a recipe for disaster. Instead, it is *essential* that this memory is allocated each time within the context in which the native add-on is initialized. This memory is typically allocated in your native add-on's `Init` method. But in some cases it can also be allocated as your native add-on is running. 

In addition to the multiple loading described above, your native add-on is also subject to automatic unloading by the JavaScript runtime engine's garbage collector when your native add-on is no longer in use.  To prevent memory leaks, any memory your native add-on has allocated *must* be freed when you native add-on is unloaded. 

The next sections describe two different techniques you can use to allocate and free persistent memory  associated with your native add-on. The techniques may be used individually or together in your native add-on.

## Instance data

> Note that the feature described here is currently experimental in Node 12.8.0 and later. 

Node-API gives you the ability to associate a single piece of memory your native-add allocates with the context under which it is running. This technique is called "instance data" and is useful when your native add-on allocates a single piece of data when its loaded. 

The `napi_set_instance_data` allows your native add-on to associate a single piece of allocated memory with the context under which you native add-on is loaded. The `napi_get_instance_data` can then be called anywhere in you native add-on to retrieve the location of the memory that was allocated. 

You specify a finalizer callback in your `napi_set_instance_data` call. The finalizer callback gets called when your native add-on is released from memory and is where you should release the memory associated with this context.  

### Resources

[Environment Life Cycle APIs](https://nodejs.org/api/n-api.html#n_api_environment_life_cycle_apis) Node.js documentation for `napi_set_instance_data` and `napi_get_instance_data`.

### Example

In this example, a number of Worker Threads are created. Each Worker Thread creates an `AddonData` struct that is tied to the Worker Thread's context using a call to `napi_set_instance_data`. Over time, the value held in the struct is incremented and decremented using a computationally expensive operation. 

In time, the Worker Threads complete their operations at which time the allocated struct is freed in the `DeleteAddonData` function. 

#### binding.c

```c
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#define NAPI_EXPERIMENTAL
#include <node_api.h>

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
  assert(napi_set_instance_data(env, result, DeleteAddonData, NULL) == napi_ok);
  return result;
}

// This function is called from JavaScript. It uses an expensive operation to
// increment the value stored inside the `AddonData` structure by one.
static napi_value Increment(napi_env env, napi_callback_info info) {
  // Retrieve the per-addon-instance data.
  AddonData* addon_data = NULL;
  assert(napi_get_instance_data(env, ((void**)&addon_data)) == napi_ok);

  // Increment the per-addon-instance value and create a new JavaScript integer
  // from it.
  napi_value result;
  assert(napi_create_int32(env,
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
  assert(napi_get_instance_data(env, ((void**)&addon_data)) == napi_ok);

  // Decrement the per-addon-instance value and create a new JavaScript integer
  // from it.
  napi_value result;
  assert(napi_create_int32(env,
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
  assert(napi_define_properties(env,
                                exports,
                                sizeof(bindings) / sizeof(bindings[0]),
                                bindings) == napi_ok);

  // Return the `exports` object provided. It now has two new properties, which
  // are the functions we wish to expose to JavaScript.
  return exports;
}
```

#### index.js

```javascript
// Example illustrating the case where a native addon is loaded multiple times.
// This entire file is executed twice, concurrently - once on the main thread,
// and once on a thread launched from the main thread.

// We load the worker threads module, which allows us to launch multiple Node.js
// environments, each in its own thread.
const {
  Worker, isMainThread
} = require('worker_threads');

// We load the native addon.
const addon = require('bindings')('multiple_load');

// The iteration count can be tweaked to ensure that the output from the two
// threads is interleaved. Too few iterations and the output of one thread
// follows the output of the other, not really illustrating the concurrency.
const iterations = 1000;

// This function is an idle loop that performs a random walk from 0 by calling
// into the native addon to either increment or decrement the initial value.
function useAddon(addon, prefix, iterations) {
  if (iterations >= 0) {
    if (Math.random() < 0.5) {
      console.log(prefix + ': new value (decremented): ' + addon.decrement());
    } else {
      console.log(prefix + ': new value (incremented): ' + addon.increment());
    }
    setImmediate(() => useAddon(addon, prefix, --iterations));
  }
}

if (isMainThread) {
  // On the main thread, we launch a worker and wait for it to come online. Then
  // we start the loop.
  (new Worker(__filename)).on('online',
      () => useAddon(addon, "Main thread", iterations));
} else {
  // On the secondary thread we immediately start the loop.
  useAddon(addon, "Worker thread", iterations);
}
```

## Cleanup hooks

> Note that the feature described here is currently available in Node-API version 3 and later. 

Your native add-on can receive one or more notifications from the Node.js runtime engine when the context in which your native-add-on has been running is being destroyed. This gives your native add-on the opportunity to release any allocated memory before the context is destroyed by the Node.js runtime engine.

The advantage of this technique is that your native add-on can allocate multiple pieces of memory to be associated with the context under which your native add-on is running. This can be useful if you need to allocate multiple memory buffers from different pieces of code as your native add-on is running. 

The drawback is that if you need to access these allocated buffer you are responsible for keeping track of the pointers yourself within the context your native add-on is running. Depending upon the architecture of your native add-on, this may or may not be an issue. 

### Resources

[Cleanup on exit of the current Node.js instance](https://nodejs.org/api/n-api.html#n_api_cleanup_on_exit_of_the_current_node_js_instance) Node.js documentation for `napi_add_env_cleanup_hook` and `napi_remove_env_cleanup_hook`.

### Example

Because keeping track of the allocated buffers is dependent upon the architecture of the native add-on, this is a trivial example showing how the buffers can be allocated and released. 

#### binding.c

```c++
#include <stdlib.h>
#include <stdio.h>
#include "node_api.h"

namespace {

void CleanupHook (void* arg) {
  printf("cleanup(%d)\n", *static_cast<int*>(arg));
  free(arg);
}

napi_value Init(napi_env env, napi_value exports) {
  for (int i = 1; i < 5; i++) {
    int* value = (int*)malloc(sizeof(*value));
    *value = i;
    napi_add_env_cleanup_hook(env, CleanupHook, value);
  }
  return nullptr;
}

}  // anonymous namespace

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
```

#### index.js

```javascript
'use strict';
// We load the native addon.
const addon = require('bindings')('multiple_load');
const assert = require('assert');
const child_process = require('child_process');
```

