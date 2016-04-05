
var addon = require('bindings')('addon');

var obj = addon.createObject(10);

console.log( obj.plusOne() );  // 11
console.log( obj.plusOne() );  // 12
console.log( obj.plusOne() );  // 13

var obj2 = addon.createObject(20);

console.log( obj2.plusOne() );  // 21
console.log( obj2.plusOne() );  // 22
console.log( obj2.plusOne() );  // 23
