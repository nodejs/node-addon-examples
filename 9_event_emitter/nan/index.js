var EventEmitter = require('events').EventEmitter,
    inherits = require('util').inherits;

var addon = require('bindings')('addon');

inherits(addon.Timer, EventEmitter);
var timer = new addon.Timer(),
    counter = 0;

timer.on('tick', function () {
    process.stdout.write('.');
    counter += 1;
});

var timeout = 5000, repeat = 200;  //ms

// Start the timer. timeout and repeat are in milliseconds.
// If timeout is zero, the callback fires on the next event
// loop iteration. If repeat is non-zero, the callback fires
// first after timeout milliseconds and then repeatedly after
// repeat milliseconds.
// examples:
//  - timeout=0    AND repeat=0   => tick once then exit
//  - timeout=5000 AND repeat=200 => wait 5sec then tick every 0.2 sec
timer.start(timeout, repeat);
var start = new Date();
console.log('starting... (press CTRL+C to stop)');
process.on('SIGINT', function () {
  timer.stop(); // will exit process by itself since event loop is now empty
  console.log('ticked %d times in %dms\n', counter, new Date()-start);
});