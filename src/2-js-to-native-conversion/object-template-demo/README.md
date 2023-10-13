The goal of this demo is to show how to implement V8 ObjectTemplate-like handlers.
NAN uses the ObjectTemplate directly: the code is adapted from NAN namedinterceptors unit test.
Node-API cannot use the ObjectTemplate: it uses the JavaScript Proxy object to get similar behavior.
