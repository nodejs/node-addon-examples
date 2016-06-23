var addon = require('bindings')('addon');

var obj = new addon.MyObject(10);
console.log( obj.plusOne() ); // 11
console.log( obj.plusOne() ); // 12
console.log( obj.plusOne() ); // 13

console.log( obj.multiply().napi_value() ); // 13
console.log( obj.multiply(10).napi_value() ); // 130

var newobj = obj.multiply(-1);
console.log( newobj.napi_value() ); // -13
console.log( obj === newobj ); // false
