---
id: special-topics.thread-safe-functions
title: Thread-safe functions
prev: special-topics.asyncworker
next: special-topics.context-awareness
---

JavaScript functions can normally only be called from a native addon's main thread. If an addon creates additional threads, then node-addon-api functions that require a `Napi::Env`, `Napi::Value`, or `Napi::Reference` must not be called from those threads.

When an addon has additional threads and JavaScript functions need to be invoked based on the processing completed by those threads, those threads must communicate with the addon's main thread so that the main thread can invoke the JavaScript function on their behalf. The thread-safe function APIs provide an easy way to do this.

A thread-safe function is created on the main thread via [ThreadSafeFunction::New](https://github.com/nodejs/node-addon-api/blob/main/doc/threadsafe_function.md#new):

```cpp
New(napi_env env,
    const Function& callback,
    const Object& resource,
    ResourceString resourceName,
    size_t maxQueueSize,
    size_t initialThreadCount,
    ContextType* context,
    Finalizer finalizeCallback,
    FinalizerDataType* data);
```

A thread-safe function encapsulates:
- Message queue: Requests to run the JavaScript function are placed in a queue, processed asynchronously by the main thread. The amount of entries allowed in the queue before returning a "queue full" error on `NonBlockingCall()` is controlled via the `maxQueueSize` parameter (specify `0` for unlimited queue)
- JavaScript function: Callback to run (`callback` parameter). This function is either (a) automatically ran with no arguments when called via the no-argument `[Non]BlockingCall()` overloads, or (b) passed as an argument to the callback function provided in the `[Non]BlockingCall(DataType* data, Callback callback)` overloads.
- Context: Optional, arbitrary data (`context` parameter) to associate with the thread-safe function.
- Finalizer: Optional callback (`finalizeCallback` parameter) to run at destruction of the thread-safe function, when all threads have finished using it.
- Finalizer data: Optional data (`data` parameter) to provide to the finalizer callback.

## Calling the Thread-Safe Function

Threads may call into JavaScript via [`[Non]BlockingCall`](https://github.com/nodejs/node-addon-api/blob/main/doc/threadsafe_function.md#blockingcall--nonblockingcall). This will add an entry to the underlying thread-safe function's queue, to be handled asynchronously on the main thread during its processing of the event loop.

## Thread Management

Multiple threads can utilize the thread-safe function simultaneously. The thread-safe function manages its lifecycle through counting the number of threads actively utilizing it. This number starts at the initial thread count parameter in `New()`, increased via `Acquire()`, and decreased via `Released()`. Once the number of active threads reaches zero, the thread-safe function is destroyed, running the finalizer callback on the main thread if provided.

Here are two general approaches to using thread-safe functions within applications:

### Known Number of Threads

If the amount of threads is known at thread-safe function creation, set the `initial_thread_count` parameter to this number in the call to `New()`. Each thread will have its own access to the thread-safe function until it calls `Release()`. Once all threads have made a call to `Release()`, the thread-safe function is destroyed.

### Creating Threads

Another common use-case is to dynamically create and destroy threads based on various logic at run-time. One way to handle this scenario is to expose several native JavaScript functions that interact with the thread-safe function APIs by:
1. Creating a thread-safe function via `New()` with initial thread count of `1`.
2. Calling `Acquire()` and creating a new native thread. The new thread can now use `[Non]BlockingCall()`.
3. Initiating cleanup/destruction, for example by ...
  - calling `Abort()` and have each thread either call `[Non]BlockingCall()` or `Release()`
  - using custom logic with other thread-safe APIs to ensure that all threads call `Release()` in order to decrease the active thread count to `0`.

## Example

This example node-addon-api module creates exposes a single function that creates a thread-safe function and a native thread. The function returns a promise that resolves after the native thread calls into JavaScript ten times.

### binding.gyp

[**binding.gyp**](https://github.com/nodejs/node-addon-examples/blob/main/thread_safe_function_counting/node-addon-api/binding.gyp)

`embed:thread_safe_function_counting/node-addon-api/binding.gyp`

### addon.cc

[**addon.cc**](https://github.com/nodejs/node-addon-examples/blob/main/thread_safe_function_counting/node-addon-api/addon.cc)

`embed:thread_safe_function_counting/node-addon-api/addon.cc`

### addon.js

[**addon.js**](https://github.com/nodejs/node-addon-examples/blob/main/thread_safe_function_counting/node-addon-api/addon.js)

`embed:thread_safe_function_counting/node-addon-api/addon.js`

Running the above script will provide output similar to:

```
2019-11-25T22:14:56.175Z 0
2019-11-25T22:14:56.380Z 1
2019-11-25T22:14:56.582Z 2
2019-11-25T22:14:56.787Z 3
2019-11-25T22:14:56.987Z 4
2019-11-25T22:14:57.187Z 5
2019-11-25T22:14:57.388Z 6
2019-11-25T22:14:57.591Z 7
2019-11-25T22:14:57.796Z 8
2019-11-25T22:14:58.001Z 9
true
```

## Frequently Asked Questions

### Q: My application isn't exiting correctly. It just hangs.

By default, Node will wait until a thread-safe function is finalized before cleaning up and exiting. See [Thread Management](#Thread-Management). This behavior can be changed via a call to `Unref()`, permitting Node to clean up without waiting for the thread count to reach zero. A call to `Ref()` will will return the threadsafe function to the previous exit behavior, requiring it to be `Release()`ed and/or `Abort()`ed by all threads utilizing it.

### Q: If a thread receives `napi_closing` from a call to `[Non]BlockingCall()`, does it still need to call `Release()`?

No. A return value of `napi_closing` should signify to the thread that the thread-safe function can no longer be utilized. This _includes_ the call to `Release()`.
