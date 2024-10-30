/**
 * @type {import('../type')}
 */
const runWorker = require('../build/Release/napi-asyncworker-example-native');

let result = runWorker.runSimpleAsyncWorker(2, AsyncWorkerCompletion);
console.log("runSimpleAsyncWorker returned '"+result+"'.");

result = runWorker.runSimpleAsyncWorker(4, AsyncWorkerCompletion);
console.log("runSimpleAsyncWorker returned '"+result+"'.");

result = runWorker.runSimpleAsyncWorker(8, AsyncWorkerCompletion);
console.log("runSimpleAsyncWorker returned '"+result+"'.");

/**
 * 
 * @param {Error | null} err 
 * @param {string | undefined} result 
 */
function AsyncWorkerCompletion (err, result) {
    if (err) {
        console.log("SimpleAsyncWorker returned an error: ", err);
    } else {
        console.log("SimpleAsyncWorker returned '"+result+"'.");
    }
};
