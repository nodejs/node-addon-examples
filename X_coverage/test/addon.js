var addon = require('bindings')('addon.node'),
    assert = require('assert');

describe('add()', function() {
  it('should add two arguments', function() {
    assert.equal(addon.add(3, 5), 8);
  });
  it('should ignore extra arguments', function() {
    assert.equal(addon.add(3, 5, 7, 9), 8);
  });
  it('should throw if not enough arguments', function() {
    assert.throws(function() {
      addon.add(3);
    }, Error);
  });
});
