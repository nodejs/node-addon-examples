
var addon = require('bindings')('addon');

addon.runCallback(function(msg) {
    console.log(msg);  // 'Hello world!'
});
