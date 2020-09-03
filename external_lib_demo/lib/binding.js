const addon = require('../build/Release/external-lib-node')

function ObjectWrapDemo (name) {
  this.greet = function (str) {
    return _addonInstance.greet(str)
  }

  const _addonInstance = new addon.ObjectWrapDemo(name)
}

module.exports = ObjectWrapDemo
