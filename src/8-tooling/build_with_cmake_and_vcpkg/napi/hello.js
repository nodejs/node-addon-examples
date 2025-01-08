var addon = require(process.cwd() + "/build/Release/scam_native.node");

console.log(addon.hello()); // 'world'
