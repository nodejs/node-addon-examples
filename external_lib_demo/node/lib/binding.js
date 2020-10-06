const externalLib = require('../build/Release/external-lib-node')

function ExternalLib (name) {
  this.greet = function (str) {
    return _instance.greet(str)
  }

  const _instance = new externalLib.ExternalLib(name)
}

module.exports = ExternalLib
