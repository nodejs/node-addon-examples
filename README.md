Node.js Addon Examples
======================

See the Node.js C++ [addons page](http://nodejs.org/docs/latest/api/addons.html) for details of the examples here.

Each directory contains a subdirectory for:

* **Node 0.10**: Mostly compatible with Node 0.8 and even 0.6, older style V8 programming
* **Node 0.12**: Newer style V8 programming, compatible with Node 0.11+
* **NAN**: The example rewritten using [NAN](https://github.com/rvagg/nan/) for compatibility with Node 0.8 -> 0.11+

You will need [node-gyp](https://github.com/TooTallNate/node-gyp/) installed as a global:

```text
$ sudo npm install node-gyp -g
```

In each example directory, run:

```text
$ npm install
$ node-gyp rebuild
$ node ./
```
