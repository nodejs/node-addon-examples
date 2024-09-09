/**
 * @type {import('../type').default}
 */
var addon = require('bindings')('addon');

addon(function(msg){
  console.log(msg); // 'hello world'
});