var addon = require('./build/Release/addon');

addon(function(msg){
  console.log(msg); // 'hello world'
});