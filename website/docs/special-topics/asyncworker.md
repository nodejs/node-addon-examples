---
id: special-topics.asyncworker
title: AsyncWorker
prev: special-topics.object-function-refs
next: special-topics.thread-safe-functions
---

## Introduction

You may have a project in which you have a piece of long-running C/C++ code that you want run in the background instead of on Node's main event loop. Node-API's [`AsyncWorker`](https://github.com/nodejs/node-addon-api/blob/main/doc/async_worker.md) class is designed specifically for this case.

As a programmer, your job is essentially to subclass `AsyncWorker` and to implement the `Execute` method. You'll also probably implement a wrapper function to make using your `AsyncWorker` easier.

In this example, we're going to create a `SimpleAsyncWorker` class that subclasses `AsyncWorker`. The worker will take an integer value indicating the length of time it is to "work." When the worker completes, it will return a text string indicating how long it worked. In one case, the worker will indicate an error instead.

## SimpleAsyncWorker

Here's the C++ header file for `SimpleAsyncWorker`:

[**SimpleAsyncWorker.h**](https://github.com/nodejs/node-addon-examples/blob/main/napi-asyncworker-example/node-addon-api/src/SimpleAsyncWorker.h)

`embed:napi-asyncworker-example/node-addon-api/src/SimpleAsyncWorker.h`

This code declares a constructor that takes as an argument the length of time (in seconds) the `SimpleAsyncWorker` is to run. A private data member is declared to hold this value.

The header also declares two methods, `Execute` and `OnOK`, which override methods declared by `AsyncWorker`, and are described in more detail below.

And here's the C++ implementation file for `SimpleAsyncWorker`:

[**SimpleAsyncWorker.cc**](https://github.com/nodejs/node-addon-examples/blob/main/napi-asyncworker-example/node-addon-api/src/SimpleAsyncWorker.cc)

`embed:napi-asyncworker-example/node-addon-api/src/SimpleAsyncWorker.cc`

The constructor takes two arguments. `callback` is the JavaScript function that gets called when the `Execute` method returns. `callback` gets called whether there was an error or not. The second constructor argument, `runTime`, is an integer value indicating how long (in seconds) the worker is to run.

Node will run the code of the `Execute` method in a thread separate from the thread running Node's main event loop. The `Execute` method _has no access_ to any part of the Node-API environment. For this reason, the `runTime` input value was stored by the constructor as a private data member.

In this implementation, the `Execute` method simply waits the number of seconds specified earlier by `runTime`. This is where the long running code goes in a real implementation. To demonstrate how error handling works, this `Execute` method declares an error when requested to run 4 seconds.

The `OnOK` method is called after the `Execute` method returns unless the `Execute` method calls `SetError` or in the case where C++ exceptions are enabled and an exception is thrown. In the case of an error, the `OnError` method is called instead of `OnOK`. The default `OnError` implementation simply calls the `AsyncWorker` callback function with the error as the only argument.

In this implementation, the `OnOK` method formulates a string value and passes it as the _second_ argument to the `callback` function specified in the constructor. The first argument passed to the `callback` function is a JavaScript `null` value. The reason for this is that a single callback function is called whether an error occurs or not. The default `OnError` method, which `SimpleAsyncWorker` does not override, passes the error as the first argument to the callback. This will become more clear in the next section.

Note that unlike `Execute`, the `OnOK` and `OnError` methods _do_ have access to the Node-API environment.

## RunSimpleAsyncWorker

We need a C++ function that instantiates `SimpleAsyncWorker` objects and requests them to be queued. This function needs to be registered with Node-API so that it is accessible from the JavaScript code.

[**RunSimpleAsyncWorker.cc**](https://github.com/nodejs/node-addon-examples/blob/main/napi-asyncworker-example/node-addon-api/src/RunSimpleAsyncWorker.cc)

`embed:napi-asyncworker-example/node-addon-api/src/RunSimpleAsyncWorker.cc`

The `runSimpleAsyncWorker` function, which is accessible from JavaScript, takes two arguments which are passed through the `info` argument. The first argument, which is passed as `info[0]`, is the `runTime` and the second argument is the JavaScript callback function which gets called when the `Execute` method returns.

The code then instantiates a `SimpleAsyncWorker` object and requests that it be queued for possible execution on the next tick. Unless the `SimpleAsyncWorker` object is queued, its `Execute` method will never be called.

Once the `SimpleAsyncWorker` object is queued, `runSimpleAsyncWorker` formulates a text string and returns it to the caller.

## Running in JavaScript

Here's a simple JavaScript program that shows how to run `SimpleAsyncWorker` instances.

[**Test.js**](https://github.com/nodejs/node-addon-examples/blob/main/napi-asyncworker-example/node-addon-api/test/Test.js)

`embed:napi-asyncworker-example/node-addon-api/test/Test.js`

In this code, the `runSimpleAsyncWorker` function is called three times, each with a different `runTime` parameter. Each call specifies `AsyncWorkerCompletion` as the callback function.

The `AsyncWorkerCompletion` function is coded to handle the cases where the `Execute` method reports an error and when it does not. It simply logs to the console when it's called.

Here's what the output looks like when the JavaScript successfully runs:

```
runSimpleAsyncWorker returned 'SimpleAsyncWorker for 2 seconds queued.'.
runSimpleAsyncWorker returned 'SimpleAsyncWorker for 4 seconds queued.'.
runSimpleAsyncWorker returned 'SimpleAsyncWorker for 8 seconds queued.'.
SimpleAsyncWorker returned 'SimpleAsyncWorker returning after 'working' 2 seconds.'.
SimpleAsyncWorker returned an error:  [Error: Oops! Failed after 'working' 4 seconds.]
SimpleAsyncWorker returned 'SimpleAsyncWorker returning after 'working' 8 seconds.'.
```

As expected, each call to `runSimpleAsyncWorker` immediately returns. The `AsyncWorkerCompletion` function gets called when each `SimpleAsyncWorker` completes.

## Caveats

- It is _absolutely essential_ that the `Execute` method makes no Node-API calls. This means that the `Execute` method has _no access_ to any input values passed by the JavaScript code.

	Typically, the `AsyncWorker` class constructor will collect the information it needs from the JavaScript objects and store _copies_ of that information as data members. The results of the `Execute` method can then be turned back into JavaScript objects in the `OnOK` method.

- The Node process is aware of all running `Execute` methods and will not terminate until all running `Execute` methods have returned.

- An AsyncWorker can be safely terminated with a call to `AsyncWorker::Cancel` from the main thread.

## Resources

[AsyncWorker Class Documentation](https://github.com/nodejs/node-addon-api/blob/main/doc/async_worker.md).

The complete source and build files for this project can be found at [inspiredware/napi-asyncworker-example](https://github.com/inspiredware/napi-asyncworker-example).
