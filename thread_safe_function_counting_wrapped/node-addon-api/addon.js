const { runAsyncWork } = require('bindings')('addon');

console.log('RunAsyncWork(): calling the C++ function.');
const promise = runAsyncWork(
  (i, s) => { console.log(`Callback from C++: even ${s}=${i}.`); },
  (i, s) => { console.log(`Callback from C++: odd ${s}=${i}.`); }
);
console.log('RunAsyncWork(): the promise is returned from C++.');
promise.then((value) => { console.log(`RunAsyncWork(): the promise resolved, from C++, to ${value}.`); });
