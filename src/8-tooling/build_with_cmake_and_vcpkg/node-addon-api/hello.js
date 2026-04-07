var addon = require(process.cwd() + "/build/Release/build-node-addon-api-with-cmake-and-vcpkg.node");

console.log(addon.hello()); // 'world'
