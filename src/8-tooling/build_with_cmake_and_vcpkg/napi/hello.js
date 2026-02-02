var addon = require(process.cwd() + "/build/Release/build-napi-with-cmake-and-vcpkg.node");

console.log(addon.hello()); // 'world'
