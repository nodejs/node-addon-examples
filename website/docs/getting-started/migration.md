---
id: getting-started.migration
title: Migration tutorial
prev: getting-started.objectwrap
---

The objective of this tutorial is to give you a good idea of the steps necessary and the tools available to migrate an existing [NAN](https://github.com/nodejs/nan) Node native add-on module to [Node-API](https://nodejs.org/api/n-api.html) using the [node-addon-api ](https://github.com/nodejs/node-addon-api) package.

This tutorial uses the conversion tool supplied with Node-API to give you a head start on the migration. However, the conversion tool will only get you so far. Further manual cleanup is still required as described below.

To keep things somewhat constrained, this tutorial uses [node-microtime](https://github.com/wadey/node-microtime) which is a simple NAN-based native add-on. This add-on makes system calls to determine the current time to microsecond resolution if supported by the operating system.

Before you start, make sure you've got all the necessary [prerequisites](prerequisites) and [tools](tools) installed.

> Node-API has been in public release and active development starting with Node 8.0.0. Since then, it's undergone a number of refinements. This tutorial has been tested with Node 10.1.0 and is known to fail with older versions of Node. You can determine the version of Node you're running with the command `node -v`.

## Clone `node-microtime`

As a first step, clone the [node-microtime](https://github.com/wadey/node-microtime) GitHub repository to your system:

```bash
git clone https://github.com/wadey/node-microtime.git
```

Before we make our modifications, it's a good idea to first build and test `node-microtime` to help verify that the necessary development tools have been correctly installed and configured.

> Since `node-microtime` has already migrated to `node-addon-api`, you have to move to tag `v2.1.9` in order to follow the tutorial.

```bash
cd node-microtime
git checkout tags/v2.1.9
npm install
npm test
```

The `npm install` command invokes the build process and `npm test` runs the code. You may see compiler warnings that do not affect the ability to run the code. When successfully built and run, you should see output that looks something like this:

```
microtime.now() = 1526334357974754
microtime.nowDouble() = 1526334357.976626
microtime.nowStruct() = [ 1526334357, 976748 ]

Guessing clock resolution...
Clock resolution observed: 1us
```

## Run the conversion tool

Once the basic operation of the code has been verified, the next step is to run the [Node-API Conversion Tool](https://github.com/nodejs/node-addon-api/blob/main/doc/conversion-tool.md). Be aware that the conversion tool _replaces files in place_. Never run the conversion tool on the only copy of your project. And obviously, you want to run it only _once_.

```
npm install --save node-addon-api
node ./node_modules/node-addon-api/tools/conversion.js ./
```

For this small project, the conversion tool runs very quickly. At this point, the conversion tool has modified the following project files:

- binding.gyp
- package.json
- src/microtime.cc

Go ahead and rebuild the converted code:

```
npm install
```

As you'll see, there are one or more compile errors that need to be addressed. There can be quite a few at times, but nothing insurmountable.

## Cleanup

The conversion tool cannot anticipate every coding situation. So there will typically be issues that need to be addressed manually. Below are the issues you're likely to encounter with this project. The best approach is to address each issue, one at a time, and attempt the `npm install` after addressing each issue until there are no more errors.

### Cannot find module 'nan'

This error, and its counterpart where `napi.h` cannot be found, is due to code missing in the `bind.gyp` file. For this project, you'll see this code in the binding.gyp:

```json
'include_dirs' : [ '<!(node -e "require(\'nan\')")' ]
```

The C/C++ include directories are still pointing to NAN. They need to point to Node-API instead. The line above should be replaced with this line:

```json
'include_dirs' : [ "<!@(node -p \"require('node-addon-api').include\")" ]
```

On other projects you may receive an error where `napi.h` cannot be found. The cause is the same as this one. The `include_dirs` property of each target must include the reference to `node-addon-api` as shown above.

### Use of undeclared identifier 'env'

The three C++ functions—`Now`, `NowDouble`, and `NowStruct`—each make a reference to the `env` variable which is not defined. `env` is meant to hold the [Node-API Environment](https://github.com/nodejs/node-addon-api/blob/main/doc/env.md) variable which is essential to nearly all Node-API calls. The value for `env` can be easily obtained from the `Napi::CallbackInfo` argument passed to each C++ function. One technique to correct this error is to add the following code as the first line in the body of each function:

```cpp
  Napi::Env env = info.Env();
```

An alternative would be to replace each instance of `env` in the three functions with `info.Env()`. The choice is yours.

### Use of `void` as the return value

Each of the three C++ functions—`Now`, `NowDouble`, and `NowStruct` are defined to return a `void` value. In fact, they should each be returning a JavaScript value. The best way to accomplish this is by replacing the `void` with `Napi::Value`. This permits each of the functions to return a JavaScript value of undetermined type. It could be any JavaScript value including String, Number, Boolean, Array, etc. Here's what they should look like:

```cpp
Napi::Value Now(const Napi::CallbackInfo&info) {
Napi::Value NowDouble(const Napi::CallbackInfo&info) {
Napi::Value NowStruct(const Napi::CallbackInfo&info) {
```

### A new way to define `exports`

Node-API uses a different technique for defining the `exports` object.

The code:

```cpp
Napi::Export(target, "now", Now);
Napi::Export(target, "nowDouble", NowDouble);
Napi::Export(target, "nowStruct", NowStruct);
```

Needs to be replaced with:

```cpp
exports.Set(Napi::String::New(env,"now"), Napi::Function::New(env, Now));
exports.Set(Napi::String::New(env,"nowDouble"), Napi::Function::New(env, NowDouble));
exports.Set(Napi::String::New(env,"nowStruct"), Napi::Function::New(env, NowStruct));
```

`exports` is a `Napi::Object` which represents a JavaScript object. The `Set` method sets the value of properties on the object and takes two arguments, the name of the property and its value. These two arguments must both be JavaScript values.

One more change is critical to the operation of Node-API. The `InitAll` function _must_ return the `exports` variable. This line must appear as the last line in the body of the function:

```cpp
return exports;
```

Failure to add this line will likely result in a segfault error at runtime.

### Replacement for `ErrnoException`

The NAN `ErrnoException` object does not exist in Node-API. The existing code looks like:

```cpp
Napi::Error::New(env, Napi::ErrnoException(errno, "gettimeofday")).ThrowAsJavaScriptException();
```

But it can easily be replaced with code that looks like this:

```cpp
std::string msg =  "gettimeofday: " + std::string(strerror(errno));
Napi::Error::New(env, msg).ThrowAsJavaScriptException();
```

## Final check

Once the code compiles without error, test the changes you've made:

```bash
npm test
```

You should see results similar to those from before the migration.

Congratulations! You've just converted your first NAN module to Node-API.

## Conclusion

Admittedly, this tutorial just scratches the surface on migrating NAN modules to Node-API. However, the basic approach is the same. Run the conversion, attempt a compile, address the errors, compile again. Wash. Rinse. Repeat.

## Resources

There are number of good resources to help you better understand Node-API as you migrate your own modules:

[The node-addon-api Home Page](https://github.com/nodejs/node-addon-api)

[node-addon-api Documentation](https://github.com/nodejs/node-addon-api#api)

[node-addon-api Examples](https://github.com/nodejs/abi-stable-node-addon-examples)

[node-addon-api Test Code](https://github.com/nodejs/node-addon-api/tree/main/test)

Each of these resources is maintained by the Node-API Team.
