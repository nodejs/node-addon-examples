const ExternalLib = require('../lib/binding.js')
const assert = require('assert')

assert(ExternalLib, 'The expected module is undefined')

function testBasic () {
  const externalLib = new ExternalLib('mr-yeoman')
  assert(externalLib.greet, 'The expected method is not defined')
  assert.strictEqual(externalLib.greet('kermit'), 'mr-yeoman', 'Unexpected value returned')
}

function testInvalidParams () {
  const externalLib = new ExternalLib()
}

assert.doesNotThrow(testBasic, undefined, 'testBasic threw an expection')
assert.throws(testInvalidParams, undefined, "testInvalidParams didn't throw")

console.log('Tests passed- everything looks OK!')
