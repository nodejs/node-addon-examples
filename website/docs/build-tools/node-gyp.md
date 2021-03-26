---
id: build-tools.node-gyp
title: node-gyp
next: build-tools.cmake-js
---

Historically, [node-gyp](https://github.com/nodejs/node-gyp) has been the build tool of choice for the Node ecosystem. The tool comes bundled with Node and is nearly universally used to build Node native modules. Most of the examples on this site use node-gyp to build the binaries.

node-gyp is based upon Google's [GYP](https://gyp.gsrc.io/) build tool. GYP, in turn, requires Python.

> node-gyp requires Python 2.7 or Python 3.5+ depending upon the operating system on which the native module is being built. The specifics of the requirements can be found [here](https://github.com/nodejs/node-gyp#installation).

For developers who find the node-gyp build tool too constraining, [CMake.js](cmake-js) is a good alternative.

### Pros

- Comes bundled with Node.
- Is nearly universally used by the Node community to build native modules.

### Cons

- The underlying GYP tool is no longer in active development by Google.
- Some developers find node-gyp too limited for their projects.
