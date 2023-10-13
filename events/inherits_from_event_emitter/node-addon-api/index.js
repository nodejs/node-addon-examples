'use strict'

const EventEmitter = require('events').EventEmitter
const NativeEmitter = require('bindings')('native_emitter').NativeEmitter
const inherits = require('util').inherits

inherits(NativeEmitter, EventEmitter)

const emitter = new NativeEmitter()

emitter.on('start', () => {
    console.log('### START ...')
})

emitter.on('data', (evt) => {
    console.log(evt)
})

emitter.on('end', () => {
    console.log('### END ###')
})

emitter.callAndEmit()
