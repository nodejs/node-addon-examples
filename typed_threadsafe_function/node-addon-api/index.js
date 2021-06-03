const { start } = require('bindings')('clock');

start.call(new Date(), function (clock) {
    const context = this;
    console.log(context, clock);
}, 5);
