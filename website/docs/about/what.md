---
id: about.what
title: What is N-API?
next: about.uses
---

## Node.js

[Node.js](https://nodejs.org/en/about/) is a command line application running on development and server systems, that encapsulates a JavaScript runtime engine along with other support code primarily targeted towards implementing command line tools and network server applications. It essentially permits you to run JavaScript on the server. It is supported on nearly all current hardware and software architectures. 

## Node Modules

Node encourages modular software development by supporting [Modules](https://nodejs.org/api/modules.html#modules_modules). Modules are essentially a colection of files and directories that conform to specific requirements. Some background information about creating Node modules can be found [here](https://docs.npmjs.com/creating-node-js-modules).

One of the real benefits of adopting Node is the comprehensive ecosystem of Node modules available to developers. The largest collection of Node modules is available from [npm](https://www.npmjs.com). The `npm` command line tool is installed along with Node. 

## Node Native Modules

Besides modules written in JavaScript, Node also provides technology to enable the deveopment of Node modules written primarily in C and C++. This permits existing C and C++ libraries to be compiled into Node *native* modules that are virtually indistinguishable from those written entirely in JavaScript. 

N-API is the technology that enables the development of Node native modules. 

## N-API

[N-API](https://nodejs.org/api/n-api.html#n_api_n_api) is a toolkit introduced in Node 8.0.0 that acts as an intermediary between C/C++ code and the Node JavaScript engine. It permits C/C++ code to access, create, and manipulate JavaScript objects as if they were created by JavaScript code. N-API is built into Node versions 8.0.0 and later and requires no further installation.

There is another Node toolkit that predates N-API, [Native Abstractions for Node.js (NAN)](https://github.com/nodejs/nan) that serves a similar purpose. NAN is implemented using direct calls to the [Chrome V8](https://developers.google.com/v8/) that Node has historically used as its JavaScript engine. The disadvantage of this approach is that the NAN layer itself, as well as code that relies on it, needs to be updated every time the V8 engine used by Node is updated. 

N-API, on the other hand, abstracts out its API from the underlying JavaScript engine. This provides two immediate benefits. The first is that N-API guarantees that the API will always be backward compatible. This means that a module you create today will continue to run on all future versions of Node without the need for running `npm install` again. Since N-API is ABI-stable, your module will continue to run even without recompilation. 

The second benefit is that your module will continue to run even if Node's underlying JavaScript engine is changed. For example, there is a lot of interest in incorporating [Microsoft's ChakraCore](https://github.com/Microsoft/ChakraCore) JavaScript engine [into Node](https://github.com/nodejs/node-chakracore). Modules built on N-API will run on the ChakraCore engine without modification. In fact, they'll run without recompilation. 

## node-addon-api

An important adjunct to N-API, although not strictly a part of the main project, is the npm [`node-addon-api`](https://www.npmjs.com/package/node-addon-api) module. The purpose of this module is to raise the N-API API from the level of "C" up to "C++". For many users, the object model implemented by `node-addon-api` makes the effort of creating N-API modules much easier and enjoyable. 