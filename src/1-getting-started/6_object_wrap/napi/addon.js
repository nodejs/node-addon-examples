var addon = require('bindings')('addon');

var obj = new addon.MyObject(10);
console.log( obj.plusOne() ); // 11
console.log( obj.plusOne() ); // 12
console.log( obj.plusOne() ); // 13

console.log( obj.multiply().value ); // 13
console.log( obj.multiply().getValue() ); // 13
var multiplyed = obj.multiply(10);
console.log( multiplyed.value ); // 130
console.log( multiplyed.getValue() ); // 130

var newobj = obj.multiply(-1);
console.log( newobj.getValue() ); // -13
console.log( obj === newobj ); // false

// use the setter
obj.value = 99;
console.log(obj.getValue()); // 99
