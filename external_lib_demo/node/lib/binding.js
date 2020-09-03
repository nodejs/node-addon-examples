const addon = require('../build/Release/external-lib-node')

function ExternalLib (name) {
  this.greet = function (str) {
    return _addonInstance.greet(str)
  }

  const _addonInstance = new addon.ExternalLib(name)
}

module.exports = ExternalLib
