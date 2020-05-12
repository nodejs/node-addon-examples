const addon = require('bindings')('addon');
const {Worker, isMainThread, workerData} = require('worker_threads');
const os = require('os');

if (isMainThread) {
    (async function main() {
        const threads = new Promise(resolve => {
            const size = os.cpus().length * 2;
            let wait = size;

            for (let i = 0; i < size; i++) {
                const worker = new Worker(__filename, {workerData: i});

                worker.on('exit', () => {
                    wait--;
                    if (wait === 0) {
                        resolve();
                    }
                });
            }
        });

        work('main');
        await threads;
    })();
} else {
    work(workerData);
}

function work(id) {
    const obj = new addon.MyObject(10);
    for (let i = 0; i < 10; i++) {
        console.log(id, obj.plusOne());
    }

    console.log(id, obj.multiply().value());
    console.log(id, obj.multiply(10).value());

    const newobj = obj.multiply(-1);
    console.log(id, newobj.value());
    console.log(id, obj === newobj);
}
