---
id: build-tools.node-pre-gyp
title: node-pre-gyp
prev: build-tools.cmake-js
next: build-tools.prebuild
---

## Introduction

One of the limitations of implementing native add-on modules is that at some point they need to be compiled and linked. In the absence of a downloadable binary, each user of a native add-on will need to compile and link the module. This requires each user to have the necessary C/C++ build tools installed.

An alternative is for the native add-on maintainer to pre-build binaries for supported platforms and architectures and to upload these binaries to a location where users can download them.

This is the specific solution offered by [node-pre-gyp](https://github.com/mapbox/node-pre-gyp).

> Note that Node-API support was added to node-pre-gyp in version 0.8.0.

> [prebuild](prebuild) is an alternative tool that addresses this same issue.

This page describes the modifications required to an Node-API add-on module in order for it to support node-pre-gyp.

## Amazon S3

By default, node-pre-gyp uploads generated binaries to the [Amazon Web Services (AWS) S3](https://aws.amazon.com/s3/) service.

> The separate [node-pre-gyp-github](https://github.com/bchr02/node-pre-gyp-github) module implements publishing binaries to GitHub. Its use is beyond the scope of this tutorial.


### Amazon S3 Requirements

Three things are required before uploading binaries to Amazon S3:

1. An Amazon Web Services account.

2. An AWS login that permits uploading to Amazon S3.

3. An [Amazon S3 bucket](https://docs.aws.amazon.com/AmazonS3/latest/dev/UsingBucket.html) to which the AWS login is permitted to upload objects.

Creating these items is covered on the [Amazon Web Services](https://aws.amazon.com) pages.

### AWS Credentials

For security reasons, login credentials, such as those for AWS, must never be stored in repositories. For development purposes, node-pre-gyp offers a couple of different ways to store the AWS credentials outside the repository as described in more detail [here](https://github.com/mapbox/node-pre-gyp#3-configure-aws-credentials):

1. Create a `~/.node_pre_gyprc` file with the following contents:

    ```json
    {
    "accessKeyId": "xxx",
    "secretAccessKey": "xxx"
    }
    ```

2. Specify environment variables like this:

    ```bash
    export node_pre_gyp_accessKeyId=xxx
    export node_pre_gyp_secretAccessKey=xxx
    ```

The [node-pre-gyp documentation](https://github.com/mapbox/node-pre-gyp#3-configure-aws-credentials) describes additional strategies that may be more appropriate for your CI and automated build environments.

### More Information

The [node-pre-gyp documentation](https://github.com/mapbox/node-pre-gyp#s3-hosting) has complete information about configuring your environments for Amazon S3.

## package.json

### The `dependencies` and `devDependencies` properties

Any module using node-pre-gyp obviously has a dependency upon node-pre-gyp. In addition, `aws-sdk` is required as a devDependency since it's the code used to upload binaries to Amazon S3.

```
"dependencies"  : {
  "node-pre-gyp": "0.10.x"
},
"devDependencies": {
  "aws-sdk": "2.x"
}
```

### The `scripts` properties

The `scripts` `install` property should specify `node-pre-gyp install`. The `--fallback-to-build` argument instructs node-pre-gyp to build the binary on the client machine if a suitable binary cannot be located to download.

```
"scripts": {
    "install": "node-pre-gyp install --fallback-to-build"
}
```

### The `binary` property

The `binary` property specifies which versions of Node-API your native add-on supports. It also instructs node-pre-gyp where your binaries are located.

```
"binary": {
    "module_name": "your_module",
    "module_path": "./lib/binding/napi-v{napi_build_version}",
    "remote_path": "./{module_name}/v{version}/{configuration}/",
    "package_name": "{platform}-{arch}-napi-v{napi_build_version}.tar.gz",
    "host": "https://your_bucket.s3-us-west-1.amazonaws.com",
    "napi_versions": [1,3]
}
```

Set the `module_name` value for your project to a valid C variable name.

The sample above shows recommended values for the `module_path`, `remote_path`, `package_name` properties. Set the appropriate bucket name and AWS region values for your project in the `host` property.

The `napi_versions` property instructs node-pre-gyp to request one or more Node-API builds. It is _required_ for Node-API add-on modules. For Node-API modules that do not require a specific Node-API version, the recommended value is `3`. If your module requires specific Node-API versions, include them in the `napi_versions` array.

The [node-pre-gyp documentation](https://github.com/mapbox/node-pre-gyp#1-add-new-entries-to-your-packagejson) has complete information including [Node-API considerations](https://github.com/mapbox/node-pre-gyp#n-api-considerations).

## binding.gyp

### New target

A new target must be added to the existing `binding.gyp` file to copy the binary built by `node-gyp` into the location specified above by `module_path`.

```
{
  "target_name": "action_after_build",
  "type": "none",
  "dependencies": [ "<(module_name)" ],
  "copies": [
	{
	  "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
	  "destination": "<(module_path)"
	}
  ]
}
```

### NAPI_VERSION

The Node-API header files configure themselves based on the C/C++ symbol `NAPI_VERSION` which can be communicated by node-pre-gyp to your C/C++ code by including the following property in the original target, typically the first one, in your `binding.gyp` file:

```
"defines": [
	"NAPI_VERSION=<(napi_build_version)",
]
```

## JavaScript updates

JavaScript code that requires the native code must be updated to dynamically locate the `.node` file.

```
var binary = require('node-pre-gyp');
var path = require('path');
var binding_path = binary.find(path.resolve(path.join(__dirname,'./package.json')));
var binding = require(binding_path);
```

## Build

Once these changes are made, you can request node-pre-gyp to build your code:

```
npm install --build-from-source
```

If you have tests configured for your project, you can also run those:

```
npm test
```

## Package and publish

The following two commands will package and publish your native add-on:

```
./node_modules/.bin/node-pre-gyp package
./node_modules/.bin/node-pre-gyp publish
```

At this point your binaries are uploaded to Amazon S3 and ready for download.

## CI and automated builds

The node-pre-gyp documentation describes in detail how to configure your module for [Travis CI](https://github.com/mapbox/node-pre-gyp#travis-automation) (Linux, macOS, and iOS) and [AppVeyor](https://github.com/mapbox/node-pre-gyp#appveyor-automation) (Windows). This gives you the ability to build, test, and publish binaries for system platforms and architectures that you may not otherwise have access to.

## Resources

[node-pre-gyp](https://github.com/mapbox/node-pre-gyp) GitHub project page

[Amazon Web Services](https://aws.amazon.com)
