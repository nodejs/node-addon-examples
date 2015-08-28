## Example X: *code coverage*

The example shows how to gather coverage of the c++ code while running
javascript tests.

It uses facilities built into gcc to measure the coverage, and the `lcov` tool
to make those measurements easy to read.


### Rerequisites

#### npm dependencies
Run `npm install` to install the npm packages for this example.

#### lcov
On Mac OS you can use something like [homebrew][] to install the `lcov`
package. On linux you can use the appropriate package manager (yum, apt-get,
etc) to install it.


### Run the Example
Simple do an `npm run-script coverage` to build the binary with coverage
instrumentation, run the tests, and gather the coverage results.


### How It Works
There are more details in the comments of `binding.gyp` and `test/coverage.sh`
but here is the basic approach:

1. build the binary with coverage
    * `gcc` has a `--coverage` flag which will cause the binary to log
      coverage measurements as it is being run.
2. generate coverage measurements
    * While running our tests (`test/addon.js`) the instrumentation built into
      the binary will cause a `build/Debug/obj.target/addon/addon.gcda` to be
      created which contains a log of lines which have been executed.
3. format the results
    * The `.gcda` file is a binary format so we need to process it so that we
      can understand the results. The `lcov` tool is used to do that. It has
      an `lcov` command for processing and a `genhtml` command to create HTML
      output.


[homebrew]: http://brew.sh
