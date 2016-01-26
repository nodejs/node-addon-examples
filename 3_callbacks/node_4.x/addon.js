const addon = require('bindings')('addon');

addon((msg) => {
  console.log(msg); // 'hello world'
});
addon(); // nothing
