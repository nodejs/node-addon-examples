---
id: build-tools.prebuild
title: prebuild
prev: build-tools.node-pre-gyp
---

## Introduction 

One of the limitations of implementing native add-on modules is that at some point they need to be compiled and linked. In the absence of a downloadable binary, each user of a native add-on will need to compile and link the module. This requires each user to have the necessary C/C++ build tools installed. 

An alternative is for the native add-on maintainer to pre-build binaries for supported platforms and architectures and to upload these binaries to a location where users can download them.

This is the specific solution offered by [prebuild](https://github.com/prebuild/prebuild#prebuild). 

> Note that Node-API support was added to prebuild in version 8.1.0.

> [node-pre-gyp](node-pre-gyp) is an alternative tool that addresses this same issue.

This page describes the modifications required to an Node-API add-on module in order for it to support prebuild.

## prebuild and prebuild-install

The capabilities offered by prebuild are actually delivered by two separate packages:

- [prebuild](https://github.com/prebuild/prebuild) — For native add-on developers.
- [prebuild-install](https://github.com/prebuild/prebuild-install) — For the users of your native add-on. 

As a developer, you will use `prebuild` to build and upload your binaries. The users of your native add-on will use `prebuild-install` to first attempt to download one of your pre-built binaries before falling back to building the binary themselves. 

## Installing

When getting started with prebuild, it's easiest to install `prebuild` as a global package:

```bash
npm install -g prebuild
```

> As you become proficient in prebuild, you may choose to alternatively install `prebuild` as one of your `devDependencies`.

## GitHub Releases

By default, prebuild uploads generated binaries as [GitHub Releases](https://help.github.com/en/github/administering-a-repository/about-releases). Creating and uploading GitHub releases requires the use of a GitHub personal access token. The steps to create a token are as follows:

- Go to the Github page to create [personal access tokens](https://github.com/settings/tokens). (You may need to log in.)

- Click `Generate new token`.

- Under `Select scopes`, your token must have either `public_repo` or `repo` scope to upload releases. 

  - Choose `public_repo` if you will be uploading only to public repositories.

  - Choose `repo` if you will be uploading to private repositories.

- Click `Generate token`.

- Make a note of the generated token as this will be the only time it is visible. 

- Create the `GITHUB_TOKEN` environment variable containing the token.  For example:

  ```
  export GITHUB_TOKEN=<github-token>
  ```

  Replace the text `<github-token>` with the token generated above.

## package.json

### The `repository` property

Since prebuild uploads your binary to GitHub, you must specify the GitHub repository in a `repository` property:

```json
  "repository": {
    "type": "git",
    "url": "https://github.com/itsmecooldude/my-napi-addon.git"
  } 
```

### The `dependencies` property

The users of your native add-on will need to install `prebuild-install` in order to download your pre-built binary. This makes `prebuild-install` a dependency:

```json
  "dependencies": {
    "prebuild-install": "^5.3.3"
  }
```

### The `scripts` properties

Here are some suggested `scripts` entries to get started:

```json
  "scripts": {
    "install": "prebuild-install --runtime napi || node-gyp rebuild",
    "rebuild": "node-gyp rebuild",
    "prebuild": "prebuild --runtime napi --all --strip --verbose",
    "upload": "prebuild --runtime napi --upload ${GITHUB_TOKEN}"
  }
```

For the `prebuild` and `prebuild-install` commands, the `--runtime` argument must be `napi` to request Node-API builds. When requesting Node-API builds, the module's `package.json` file _must_ include a `binary` property as described next. And the `binding.gyp`  `CMakeLists.txt` file _must_ include a define for `NAPI_VERSION` as described below.

One or more `--target` arguments may be specified to request builds for specific Node-API versions. Node-API versions are positive integer values. Alternatively, `--all` may be used to request builds for all Node-API versions supported by the module. 

In the absence of both `--target` and `--all` arguments, `prebuild` will build the most current version of the module supported by the Node instance performing the build. 

### The `binary` property

Native modules that are designed to work with [Node-API](https://nodejs.org/api/n-api.html) must explicitly declare the Node-API version(s) against which they can build. This is accomplished by including a `binary` property on the module's `package.json` file. For example:

```json
"binary": {
  "napi_versions": [2,3]
}
```

In the absence of a need to compile against a specific Node-API version, the value `3` is a good choice as this is the Node-API version that was supported when Node-API left experimental status. 

Modules that are built against a specific Node-API version will continue to operate indefinitely, even as later versions of Node-API are introduced.

## NAPI_VERSION

The Node-API header files supplied with Node use the `NAPI_VERSION` preprocessor value supplied by the user to configure each build to the specific Node-API version for which the native addon is being built. In addition, the module's C/C++ code can use this value to conditionally compile code based on the Node-API version it is being compiled against.

`prebuild` supports two build backends: [`node-gyp`](https://github.com/nodejs/node-gyp) and [`cmake-js`](https://github.com/cmake-js/cmake-js). The `NAPI_VERSION` value is configured differently for each backend. 

### node-gyp

The following code must be included in the `binding.gyp` file of modules targeting Node-API:

```json
"defines": [
  "NAPI_VERSION=<(napi_build_version)"
]
```

### cmake-js

The following line must be included in the `CMakeLists.txt` file of modules targeting Node-API:

```cmake
add_compile_definitions(NAPI_VERSION=${napi_build_version})
```

## Building and uploading your binary

Using the `package.json` `scripts` entries as a starting point, this command will build and package your native add-on:

```
npm run prebuild
```

You can run this command as often as necessary.

When you're ready, run this command to upload your compiled binary to GitHub:

```
npm run upload
```

> This command requires that the `GITHUB_TOKEN` environment variable contains a valid GitHub token as described above.

Once you become proficient in prebuild, you can modify the `package.json` `scripts` entries to meet your specific needs. 

## Life for your users

You've declared `prebuild-install` as a dependency. The `install` script in your `package.json` file looks like this:

```json
    "install": "prebuild-install --runtime napi || node-gyp rebuild"
```

When a user installs your native add-on, the `prebuild-install` tool first looks to see if it can locate a suitable pre-built binary on GitHub for the user's specific Node-API version, architecture, and platform. If the binary is found, `prebuild-install` downloads, unpacks, and installs the binaries in the correct location. 

If the binary is not found, the `install` script falls back to whatever is specified after the `||`. In this case, a `node-gyp rebuild` is triggered which will attempt to build the binary on the user's machine.

## Resources

[prebuild](https://github.com/prebuild/prebuild) Project GitHub page

[prebuild-install](https://github.com/prebuild/prebuild-install) Project GitHub page

