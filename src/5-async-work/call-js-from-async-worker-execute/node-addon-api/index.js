'use strict'

const { EventEmitter } = require('node:events');
/**
 * @type {import('./type')}
 */
const { dispatch } = require('bindings')('dispatcher');

class Socket extends EventEmitter {
  constructor () {
    super();
    dispatch(this.emit.bind(this));
  }
}

const socket = new Socket();

socket.on('open', () => {
  console.log('opened');
});

socket.on('message', (message) => {
  console.log(`message: ${message}`);
});

socket.on('close', () => {
  console.log('closed');
});
