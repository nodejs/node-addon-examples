var addon = require('bindings')('addon');
var util = require('util');
console.log(util.inspect(addon.MyObject), {showHidden: true, depth: null });
console.log(util.inspect(addon.MyObject.prototype), {showHidden: true, depth: null });
addon.MyObject.prototype.test = function() {
}

var obj = new addon.MyObject(10);
console.log(obj);

obj.test();

console.log( obj.plusOne() ); // 11
console.log( obj.plusOne() ); // 12
console.log( obj.plusOne() ); // 13

console.log( obj.multiply().value() ); // 13
console.log( obj.multiply(10).value() ); // 130

var newobj = obj.multiply(-1);
console.log( newobj.value() ); // -13
console.log( obj === newobj ); // false
