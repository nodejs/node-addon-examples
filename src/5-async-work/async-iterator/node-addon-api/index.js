/**
 * @type {import('./type')}
 */
const { AsyncIteratorExample } = require('bindings')('example');

/**
 * 
 * @param {number} from 
 * @param {number} to 
 */
async function main(from, to) {
    const iterator = new AsyncIteratorExample(from, to);
    for await (const value of iterator) {
        console.log(value);
    }
}

/*
// The JavaScript equivalent of the node-addon-api C++ code for reference
async function main(from, to) {
    class AsyncIteratorExample {
        constructor(from, to) {
            this.from = from;
            this.to = to;
        }

        [Symbol.asyncIterator]() {
            return {
                current: this.from,
                last: this.to,
                next() {
                    return new Promise(resolve => {
                        setTimeout(() => {
                            if (this.current <= this.last) {
                                resolve({ done: false, value: this.current++ });
                            } else {
                                resolve({ done: true });
                            }
                        }, 1000)
                    });
                }
            }
        }
    }
    const iterator = new AsyncIteratorExample(from, to);

    for await (const value of iterator) {
        console.log(value);
    }
}
*/

main(0, 5)
    .catch(e => {
        console.error(e);
        process.exit(1);
    });
