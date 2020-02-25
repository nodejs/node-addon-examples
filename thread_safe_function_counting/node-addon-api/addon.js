const { createTSFN } = require('bindings')('addon');

const callback = (...args) => { 
    console.log(new Date, ...args); 
};

void async function() {
    console.log(await createTSFN(callback));
}();
