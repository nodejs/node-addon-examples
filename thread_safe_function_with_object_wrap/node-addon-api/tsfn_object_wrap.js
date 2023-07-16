const TsfnObjectWrap = require('bindings')('tsfn_object_wrap.node')
const x = new TsfnObjectWrap((value) => {
  // Do something, anything, with x to keep it in scope, otherwise the instance
  // will be collected and the process will exit.
  x.someProperty = value;

  console.log('JS1: Called with ' + value);
  return -value;
});

const y = new TsfnObjectWrap((value) => {
  // Do something, anything, with y to keep it in scope, otherwise the instance
  // will be collected and the process will exit.
  y.someProperty = value;

  console.log('JS2: Called with ' + value);
  return -value;
});
