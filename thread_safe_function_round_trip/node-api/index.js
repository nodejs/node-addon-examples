const bindings = require('bindings')('round_trip');

bindings.startThread((item, thePrime) => {
  console.log('The prime: ' + thePrime);

  // Answer the call with a 90% probability of returning true somewhere between
  // 200 and 400 ms from now.
  setTimeout(() => {
    const theAnswer = (Math.random() > 0.1);
    console.log(thePrime + ': answering with ' + theAnswer);
    bindings.registerReturnValue(item, theAnswer);
  }, Math.random() * 200 + 200);
});
