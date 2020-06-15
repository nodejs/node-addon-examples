---
id: special-topics.asynccontext
title: AsyncContext
---

## Background 

For many simple projects requiring support for asynchronous operations, `AsyncWorker`, which relies on the worker threads support built into Node.js is sufficient. However, other projects that require OS native threads must coordinate these threads with the Node.js runtime using the `AsyncContext` class.

The `AsyncContext` is passed to `MakeCallback`. 