---
id: getting-started.helloworld
title: Hello, world!
---

**2018-11-06** This file was created in anticipation of NodeConf EU 2018 when it was not clear whether `generator-napi-module` was going to be updated in time for the Node-API workshop at the conference. It turns out that `generator-napi-module` did land in time, so this file has never been used and is not included in the `toc.json` file. 

# Hello, world!

This tutorial will walk you through the process of creating a simple Node-API module from scratch. It requires no dependencies beyond Node and npm itself. 

Before you start, make sure you've got all the necessary [prerequisites](prerequisites) and [tools](tools) installed.

As you select where to begin, you should be aware that Node-API operates at two levels which we can think of as the "C level" and the "C++ level". 

The "C level" code is built entirely into Node itself and is very well documented on the [Node documentation pages](https://nodejs.org/api/n-api.html). If you need low-level access to the intricacies of Node, this is the tool for you. 

Alternatively, there is the [node-addon-api](https://github.com/nodejs/node-addon-api) package which adds a C++ wrapper to the Node-API code built into Node. This package makes working with Node-API much easier as it implements a very nice object model and abstracts away much of the detailed coding that would otherwise be required, while retaining the Node-API promise of ABI stability and forward compatibility.

This tutorial uses `node-addon-api`.

> Node-API has been in public release and active development starting with Node 8.0.0. Since then, it's undergone a number of refinements. This tutorial has been tested with Node 8.9.4 and 9.5.0 and is known to fail with older versions of Node. You can determine the version of Node you're running with the command `node -v`.

## Creating a project

The first step is to create a directory to hold your project:

```bash
mkdir hello-world
cd hello-world
```

Next run `npm init` to create a bare-bones `package.json` file in your project directory:

```bash
npm init
```

Here are the prompts you'll see and some suggested responses:

```
package name: (hello-world) 
version: (1.0.0) 
description: A simple Node-API example
entry point: (index.js) hello-world.js
test command: 
git repository: 
keywords: 
author: Your name goes here
license: (ISC) 
```

At this point your project directory should contain a simple `package.json` file populated with the information you entered above. 

## package.json

This project uses the [node-addon-api](https://github.com/nodejs/node-addon-api) package which adds a really nice C++ wrapper to the Node-API code built into Node. This dependency needs to be added to the `package.json` file:

```json
"dependencies": {
  "node-addon-api": "^1.5.0"
}
```

This project uses the `gtp` build system, so this needs to also be specified in the `package.json` file:

```JSON
"gypfile": true
```

## binding.gyp

One of the challenges of making C/C++ code available to Node is getting the code compiled, linked, and packaged for a variety of operating systems and architectures. Historically, this would require learning the intricacies of a variety of build tools across a number of operating systems. This is the specific issue GYP seeks to address.  

Using [GYP](https://gyp.gsrc.io/index.md) permits having a single configuration file that works across all platforms and architectures GYP supports. (It's GYP, by the way, that requires Python).

[node-gyp](https://github.com/nodejs/node-gyp) is a command line tool built in Node that orchestrates GYP to compile your C/C++ files to the correct destination. When npm sees the `"gypfile": true` entry in your `package.json` file, it automatically invokes its own internal copy of `node-gyp` which looks for this `binding.gyp` file which must be called `binding.gyp` in order for node-gyp to locate it.

The `binding.gyp` file is a GYP file which is thoroughly documented [here](https://gyp.gsrc.io/docs/UserDocumentation.md). There is also specific information about building libraries [here](https://gyp.gsrc.io/docs/UserDocumentation.md#skeleton-of-a-typical-library-target-in-a-gyp-file).

Here's a  `binding.gyp` file that will suffice for this project:

{% include_code binding.gyp lang:JavaScript napi-hello-world/binding.gyp %} 

## hello-world.cc

{% include_code src/hello-world.cc lang:cc napi-hello-world/hello-world.cc %}

Here is the nub of our project where all the magic occurs. This is a sample C++ file that shows how to use the power of the `node-addon-api` package to access, create, and manipulate JavaScript objects in C++. 

The `napi.h` file included in the header file comes from `node-addon-api`. This is the C++ wrapper that declares a number of C++ classes representing JavaScript primitive data types and objects. 

This file declares a C++ `Greet` function that returns a `Napi::String` value. 

The C++ `Init` function declares the "exports" of the module. These are analogous to the exports declared by traditional JavaScript modules. This module exports the `Greet` function as declared above in the code.

The macro call at the bottom of the C++ file, `NODE_API_MODULE(greet, Init)`, specifies that the `Init` function is to be called when the module is loaded.  

## hello-world.js

{% include_code hello-world.js lang:JavaScript napi-hello-world/hello-world.js %}

This JavaScript file shows how to load and run the code in the C++ binary. 

The module is loaded exactly like any other module using the `require` command. Once loaded, the exports of the modules can then be accessed. In this case, the sole export is the `greet` function. 

## Build and running

Once the files are created, the code can be build and run using the following commands:

```bash
npm install node-addon-api
npm install
node hello-world.js
```

The `npm install node-addon-api` command insures that the `node-addon-api` module is installed. `node-addon-api` must be installed before the next step.

The second `npm install`  builds your native module and insures all other dependencies are installed. 

The `node hello-world.js` command runs the JavaScript code that accesses the binary built in the previous step. The output should look like this:

```
world
```

## Conclusion

This project provides a brief introduction on creating a binary module that exports a single function. In addition, here are some things you might want to try:

- Run `hello-world.js` in your debugger. See if you can step through the code to get a better understanding of how it works. What sort of visibility are you getting into the binary module created by the C++ code?

- Modify the C++ `Greet` function to change its behavior or add additional arguments.  

- Modify the C++ `Greet` function to return a JavaScript object instead of a string.  

- Modify `hello-world.cc` to export additional functions.  

## Resources

- [node-addon-api](https://github.com/nodejs/node-addon-api) Documentation

- The [node-gyp](https://www.npmjs.com/package/node-gyp) Package

- [GYP](https://gyp.gsrc.io) and [.gyp file](https://gyp.gsrc.io/docs/UserDocumentation.md) Documentation.

