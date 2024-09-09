/**
 * @type {import('./type')}
 */
const addon = require('../build/Release/object-wrap-demo-native');
/**
 * 
 * @param {string} name 
 */
function ObjectWrapDemo(name) {
    this.greet = function(str) {
        return _addonInstance.greet(str);
    }

    var _addonInstance = new addon.ObjectWrapDemo(name);
}

module.exports = ObjectWrapDemo;
