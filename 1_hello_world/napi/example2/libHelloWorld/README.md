This example covers handling UTF-8 encoded strings as parameters for an external C library

Execute build.sh it will execute ./lib/build.sh wich compiles die libHelloWorld.so and copy it with the header to /usr/lib and /usr/include
After that it ties to build die NAPI Module and execute the module.js.

The output should be: "Hello concat World equals HelloWorld"
