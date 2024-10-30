// Use the "bindings" package to locate the native bindings.
/**
 * @type {import('./type')}
 */
const binding = require('bindings')('binding');

// Call the function "startThread" which the native bindings library exposes.
// The function accepts a callback which it will call from the worker thread and
// into which it will pass prime numbers. This callback simply prints them out.
binding.startThread((thePrime) =>
  console.log("Received prime from secondary thread: " + thePrime));
