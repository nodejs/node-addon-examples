Node.js Addon Examples
=========================================
[![Build Status](https://travis-ci.com/nodejs/node-addon-examples.svg?branch=master)](https://travis-ci.com/nodejs/node-addon-examples)

**A repository of [Node.js Addons](https://nodejs.org/api/addons.html#addons_c_addons) examples.**

Implementations of examples are named either after Node.js versions (`node_0.10`,
`node_0.12`, etc), or Node.js addon implementation APIs:
- [`nan`](https://github.com/nodejs/nan): C++-based abstraction between Node and direct V8 APIs.
- [`napi`](https://nodejs.org/api/n-api.html): C-based API guaranteeing [ABI stability](https://nodejs.org/en/docs/guides/abi-stability/) across different node versions as well as JavaScript engines.
- [`node-addon-api`](https://github.com/nodejs/node-addon-api): header-only C++ wrapper classes which simplify the use of the C-based N-API.

Implementations against unsupported versions of Node.js are provided for
completess and historical context. They are not maintained.

## Usage

The directory structure is as follows:

```
<name of example>
  |
  +--- <implementation 1>
  |      |
  |      +--- files...
  +--- <implementation 2>
  .      |
  .      +--- files...
  .
```


In each example's implementation subdirectory, run

```text
$ npm install
$ node ./
```

to see the example in action.
