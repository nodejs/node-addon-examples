/**
 * @type {import('../type').default}
 */
var addon = require('bindings')('addon');

var fn = addon();
console.log(fn()); // 'hello world'
