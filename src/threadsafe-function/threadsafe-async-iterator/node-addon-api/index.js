const { ThreadSafeAsyncIteratorExample } = require('bindings')('example');

async function main(from, to) {
    const iterator = new ThreadSafeAsyncIteratorExample(from, to);
    for await (const value of iterator) {
        console.log(value);
    }
}

main(0, 5)
    .catch(e => {
        console.error(e);
        process.exit(1);
    });
