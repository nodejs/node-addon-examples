Node.js Addon Examples
======================

See the Node.js C++ [addons page](http://nodejs.org/docs/latest/api/addons.html) for details of the examples here.

In each directory, run:

```sh
$ node-gyp rebuild
$ node ./
```

**See the [v0.11](https://github.com/rvagg/node-addon-examples/tree/v0.11/) branch for updated examples applicable to Node v0.11 and above.**

## Compatibility notes

### Node v0.11: V8 requires current isolate

The V8 upgrade that occured when v0.11 was released requires that the current *isolate* be passed to scope creation calls and most `New()` calls.

```c++
Isolate* isolate = Isolate::GetCurrent();
// ...
HandleScope scope(isolate);
// ...
Persistent<Function> constructor = Persistent<Function>::New(isolate, tpl->GetFunction());
```

Omission of the current isolate will only trigger a compile-time warning at this stage but addon authors wishing to remove those warnings and remain backward-compatible with v0.10 and prior may need to get creative with macros:

```c++
// node::node_isolate can be used to fetch a reference to Node's Isolate
// NODE_MODULE_VERSION was incremented for v0.11

#if NODE_MODULE_VERSION > 0x000B
#  define MY_NODE_ISOLATE_DECL Isolate* isolate = Isolate::GetCurrent();
#  define MY_NODE_ISOLATE node::node_isolate
#  define MY_NODE_ISOLATE_PRE node::node_isolate, 
#  define MY_NODE_ISOLATE_POST , node::node_isolate 
#else
#  define MY_NODE_ISOLATE_DECL
#  define MY_NODE_ISOLATE
#  define MY_NODE_ISOLATE_PRE
#  define MY_NODE_ISOLATE_POST
#endif

MY_NODE_ISOLATE_DECL
HandleScope scope(MY_NODE_ISOLATE);
// ...
Persistent<Function> constructor = Persistent<Function>::New(MY_NODE_ISOLATE_PRE tpl->GetFunction());
```

----------------------------------------------------
