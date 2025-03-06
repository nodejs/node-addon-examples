const addon = require('bindings')('hello');

console.log(addon.hello()); // 'world'
