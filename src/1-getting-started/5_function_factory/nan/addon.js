const addon = require('bindings')('addon');

const fn = addon();
console.log(fn()); // 'hello world'