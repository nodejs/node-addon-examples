/**
 * @type {import('../type')}
 */
var addon = require('bindings')('addon');

var obj1 = addon.createObject(10);
var obj2 = addon.createObject(20);
var result = addon.add(obj1, obj2);

console.log(result); // 30
