var addon = require('./build/Release/addon');

var fn = addon();
console.log(fn()); // 'hello world'