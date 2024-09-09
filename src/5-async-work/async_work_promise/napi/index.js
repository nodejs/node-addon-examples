// Use the "bindings" package to locate the native bindings.
/**
 * @type {import('../type')}
 */
const binding = require('bindings')('binding');

// Call the function "startWork" which the native bindings library exposes.
// The function returns a promise which will be resolved at the complete of the
// work with an array of worked out primes. This resolution simply prints them out.
binding.startWork()
  .then((thePrimes) => {
    console.log("Received primes from completed work: " + thePrimes)
  });
