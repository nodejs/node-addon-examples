# N-API External Lib Demo

This is an example project that demonstrates how to wrap an external C++ library (i.e. one that we do not own ourselves) in a Node module.
The example assumes the following folder structure:

```bash
root  # project root folder
  +- src  # the sources of the external lib
  |    +- external_lib.cpp  # the api of the external lib
  |    +- external_lib.h
  |    +- ...  # other source files of the external lib
  +- node  # the node wrapper addon
  +- ...   # wrappers for other languages
```

To build and run this program on your system, clone it to your computer and run these two commands inside your clone:

```
npm install
npm test
```

> You need to have Node 10.5.0 or later installed. 