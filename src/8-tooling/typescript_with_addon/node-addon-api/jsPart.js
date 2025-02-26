/**
 * @type {import('./type')}
 */
const addon = require('bindings')('typescript_with_addon');

function javascriptPart(value) {
  const result = addon.cppPartExportedByNapi(++value);

  console.log('I\'m a Javascript function and I\'m sending', 
  value,
  'to C++ and I received',
  result);

  return result;
}

module.exports = {
  javascriptPart
}