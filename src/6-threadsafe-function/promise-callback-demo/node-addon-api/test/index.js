const { PromiseCallbackDemo } = require('../build/Release/promise-callback-demo');

let shouldFail = false;
// Resolve or reject a promise with an ISO date format after 100 milliseconds
function jsCallback() {
    return new Promise((resolve, reject) => setTimeout(() => {
        ((shouldFail = !shouldFail) ? reject : resolve)(new Date().toISOString());
    }, 100));
}

new PromiseCallbackDemo(jsCallback);
