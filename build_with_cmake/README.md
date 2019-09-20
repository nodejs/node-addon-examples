## Building N-API Addons Using CMake.js

### Examples

The objective of these examples is to demonstrate how to build N-API addons using [CMake.js](https://github.com/cmake-js/cmake-js#readme).

These example projects assume that CMake.js has been installed globally:

```
npm install -g cmake-js
cmake-js --help
```

Then, in each of the `napi` and `node-addon-api` directories, the following commands build and test each addon:

```
npm install
npm test
```

Complete CMake.js documentation can be found on the [CMake.js GitHub repository](https://github.com/cmake-js/cmake-js#readme).

### NAPI_VERSION

When building N-API addons, it's important to specify to the build system the N-API version your code is designed to work with. With CMake.js, this information is specified in the `CMakeLists.txt` file:  

```
add_definitions(-DNAPI_VERSION=3)
```

Since N-API is ABI-stable, your N-API addon will work, without recompilation, with the N-API version you specify in `NAPI_VERSION` and all subsequent N-API versions. 

In the absence of a need for features available only in a specific N-API version, version 3 is a good choice as it is the version of N-API that was active when N-API left experimental status. 