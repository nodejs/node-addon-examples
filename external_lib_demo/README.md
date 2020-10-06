[![Version 0.0.1](https://img.shields.io/badge/version-0.0.1-blue)](https://github.com/KaiRoesner/node-addon-examples/tree/rel-0.0.1/external_lib_demo#n-api-external-lib-demo)

# N-API External Lib Demo

This is an example project that demonstrates how to wrap an external C++ library (i.e. one that we do not own ourselves) in a Node module.
The example assumes the following folder structure:

```bash
root  # project root folder
  +- src  # the sources of the external lib
  |    +- external_lib.cpp  # the api of the external lib
  |    +- external_lib.h
  |    +- ...  # other source files of the external lib
  |    +- wrapper.cc  # N-API wrapper of the external lib
  |    +- wrapper.h
  +- node  # the node wrapper addon
  +- ...   # wrappers for other languages
```

To build and run this program on your system, clone it to your computer and run these commands inside the `node` folder of your clone:

```bash
$ npm install
$ npm run test
```

> You need to have Node 10.5.0 or later installed. 

To re-build the program after changing the C++ code do

```bash
$ npm run rebuild
```
