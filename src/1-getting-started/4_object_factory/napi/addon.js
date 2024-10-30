/**
 * @type {import('../type').default}
 */
var addon = require('bindings')('addon');

var obj1 = addon('hello');
var obj2 = addon('world');
console.log(obj1.msg+' '+obj2.msg); // 'hello world'
