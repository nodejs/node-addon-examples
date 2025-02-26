const addon = require('bindings')('addon');

const obj1 = addon('hello');
const obj2 = addon('world');
console.log(obj1.msg+' '+obj2.msg); // 'hello world'
