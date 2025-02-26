'use strict'

/**
 * @type {import('./type')}
 */
const { NativeAddon } = require('bindings')('addon')

function JSFnRef() {
    console.log('Hi! I\'m a JS function reference!')
}

function JSFn() {
    console.log('Hi! I\'m a JS function!')
}

const ITERATIONS = 5

const addon = new NativeAddon(JSFnRef, JSFn)

for (let i = 0; i < ITERATIONS; i++) {
    addon.tryCallByStoredReference()
}

try {
    addon.tryCallByStoredFunction()
} catch (err) {
    console.error('This code crashes because JSFn is valid only inside the constructor function.')
    console.error('The lifespan of the JSFn function is limited to the execution of the constructor function.')
    console.error('After that, the function stored in JSFn is ready to be garbage collected and it cannot be used anymore.')
}
