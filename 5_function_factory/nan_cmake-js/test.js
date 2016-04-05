
var addon = require('bindings')('addon');

var fn = addon.createFunction();
console.log(fn());  // 'Hello world!'
