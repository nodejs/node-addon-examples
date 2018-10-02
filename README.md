Node.js Addon Examples
=========================================

This repository is the home of the Addon Examples.

The directory structure is as follows:

```
<name of example>
  |
  +--- <implementation 1>
  |      |
  |      +--- files...
  +--- <implementation 2>
  .      |
  .      +--- files...
  .
```

Implementations are named either after Node.js versions (`node_0.10`,
`node_0.12`, etc), or Node.js addon implementation APIs (`nan`, `napi`,
or `node-addon-api`).

Implementations against unsupported versions of Node.js are provided for
completess and historical context. They are not maintained.

In each example's implementation subdirectory, run

```text
$ npm install
$ node ./
```

to see the example in action.
