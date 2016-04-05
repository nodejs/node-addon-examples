
var addon = require('bindings')('addon');

var obj1 = addon.createObject('Hello');
var obj2 = addon.createObject('world');

console.log(obj1.msg + ' ' + obj2.msg + '!');  // 'Hello world!'
