/**
 * @type {import('../type')}
 */
const addon = require('bindings')('object-template-demo');

const interceptor = addon.create();
console.log(interceptor.prop); // 'foo'
interceptor.prop = 'setting a value';
console.log(interceptor.prop); // 'setting a value'
delete interceptor.something;
console.log(interceptor.prop); // 'goober';
console.log(Object.prototype.hasOwnProperty.call(interceptor, "thing")); // true
console.log(Object.keys(interceptor)[0]); // 'value'
