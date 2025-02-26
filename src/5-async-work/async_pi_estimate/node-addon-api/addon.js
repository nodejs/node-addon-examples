const addon = require('bindings')('addon');
const calculations = process.argv[2] || 100000000;

function printResult(type, pi, ms) {
  console.log(type, 'method:');
  console.log('\tπ ≈ ' + pi +
              ' (' + Math.abs(pi - Math.PI) + ' away from actual)');
  console.log('\tTook ' + ms + 'ms');
  console.log();
}

function runSync() {
  const start = Date.now();
  // Estimate() will execute in the current thread,
  // the next line won't return until it is finished
  const result = addon.calculateSync(calculations);
  printResult('Sync', result, Date.now() - start);
}

function runAsync() {
  // how many batches should we split the work in to?
  const batches = process.argv[3] || 16;
  let ended = 0;
  let total = 0;
  const start = Date.now();

  function done (err, result) {
    total += result;

    // have all the batches finished executing?
    if (++ended === batches) {
      printResult('Async', total / batches, Date.now() - start);
    }
  }

  // for each batch of work, request an async Estimate() for
  // a portion of the total number of calculations
  for (let i = 0; i < batches; i++) {
    addon.calculateAsync(calculations / batches, done);
  }
}

runSync();
runAsync();
