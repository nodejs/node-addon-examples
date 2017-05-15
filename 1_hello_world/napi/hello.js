var addon = require('bindings')('hello');

console.log(JSON.stringify(addon)); // 'world'
console.log(addon.hello()); // 'world'
