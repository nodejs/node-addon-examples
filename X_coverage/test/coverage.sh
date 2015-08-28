#!/bin/bash

# For this example we'll just use `coverage/` but it can be anything you want.
COVERAGE_DIR=coverage

# Stop this script if any command fails.
set -e

# Reset the coverage directory.
rm -rf $COVERAGE_DIR
mkdir -p $COVERAGE_DIR

# Build the binary with coverage instrumentation built into it.
# See binding.gyp for details on how the --coverage flag is used.
node-gyp rebuild --debug --coverage=true

# Run the tests. The instrumentation built into the binary will cause a
# `build/Debug/obj.target/addon/addon.gcda` to be created, which contains the
# coverage measurements.
node_modules/.bin/mocha test/addon.js

# Turn the binary `*.gcda` file into a standard `lcov.info` text file.
lcov --capture --directory build --output-file $COVERAGE_DIR/lcov.info

# Strip out coverage measurements for things which are not our code.
lcov --remove $COVERAGE_DIR/lcov.info \
  "/usr/*" \
  "node_modules/*" \
  "v8/include/*" \
  --output-file $COVERAGE_DIR/lcov.info

# Print coverage summary to STDOUT, which is nice quick feedback.
lcov --list $COVERAGE_DIR/lcov.info

# Create the HTML files with details of coverage measurements.
genhtml --output-directory $COVERAGE_DIR/lcov-report $COVERAGE_DIR/lcov.info

echo
echo "open $COVERAGE_DIR/lcov-report/index.html in a browser for coverage details"

