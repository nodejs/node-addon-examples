This example covers handling of UTF-8 encoded strings as parameters for an external C library. It goes beyond the demonstration of the initial example, seen in the `example1` folder. Combining both examples together should result in a clearer and more complete view of how to use the NAPI for this type of use cases.

1. Dependencies
```
gcc
python2
nodejs
node-gyp
```
Installable on Debian-based systems like this:
```
sudo apt install build-essential python2 nodejs node-gyp
```

2. Building and running the project:
```
cd node-addon-examples/1_hello_world/napi/example2/libHelloWorld/
./build.sh
```
This will execute `build.sh` which will compile `libHelloWorld.so` within `lib/`. Afterwards, the shared object will be copied to `/usr/lib` and its header will be copied to `/usr/include`. Done that, the NAPI module will be built and `module.js` will be executed.

3. Once the previous steps were finished successfully, the output should be as follows:
```
Hello concat World equals HelloWorld
```
