var addon = require('bindings')('build-napi-with-cmake');

console.log(addon.hello()); // 'world'
