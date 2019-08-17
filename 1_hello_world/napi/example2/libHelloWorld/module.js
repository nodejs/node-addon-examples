const addon = require('./build/Release/module');
const hello = "Hello";
const world = "World";
 
console.log(`${hello} concat ${world} equals`, addon.helloWorldConcat(hello, world));
