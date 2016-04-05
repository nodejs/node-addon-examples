var addon = require('bindings')('addon')

console.log('This should be eight:', addon.add(3, 5))