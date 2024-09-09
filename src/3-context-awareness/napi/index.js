// Example illustrating the case where a native addon is loaded multiple times.
// This entire file is executed twice, concurrently - once on the main thread,
// and once on a thread launched from the main thread.

// We load the worker threads module, which allows us to launch multiple Node.js
// environments, each in its own thread.
const {
  Worker, isMainThread
} = require('worker_threads');

// We load the native addon.
/**
 * @type {import('../type')}
 */
const addon = require('bindings')('multiple_load');

// The iteration count can be tweaked to ensure that the output from the two
// threads is interleaved. Too few iterations and the output of one thread
// follows the output of the other, not really illustrating the concurrency.
const iterations = 1000;

/**
 * This function is an idle loop that performs a random walk from 0 by calling
 * into the native addon to either increment or decrement the initial value.
 * 
 * @param {import('../type')} addon 
 * @param {string} prefix 
 * @param {number} iterations 
 */
function useAddon(addon, prefix, iterations) {
  if (iterations >= 0) {
    if (Math.random() < 0.5) {
      console.log(prefix + ': new value (decremented): ' + addon.decrement());
    } else {
      console.log(prefix + ': new value (incremented): ' + addon.increment());
    }
    setImmediate(() => useAddon(addon, prefix, --iterations));
  }
}

if (isMainThread) {
  // On the main thread, we launch a worker and wait for it to come online. Then
  // we start the loop.
  (new Worker(__filename)).on('online',
      () => useAddon(addon, "Main thread", iterations));
} else {
  // On the secondary thread we immediately start the loop.
  useAddon(addon, "Worker thread", iterations);
}
