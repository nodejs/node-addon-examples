/** @type {import('../type')} */
var addon = require('bindings')('hello');

console.log(addon.hello()); // 'world'