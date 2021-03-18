---
id: getting-started.tools
title: The tools you’ll need
prev: getting-started.prerequisites
next: getting-started.first
---

## Node

[Node](https://nodejs.org/en/about/) is the software that runs JavaScript on your computer. It bundles the [Chrome V8 JavaScript engine](https://developers.google.com/v8/) with a set of other tools that permit you to run your JavaScript code outside of the browser.

The easiest way to install Node on your system is to download the appropriate installer from [here](https://nodejs.org/en/). The LTS (Long Term Support) version is the most stable version and is most likely the one you want. The Current version is the latest available and includes experimental features. The Windows, macOS, and Linux installers also include a relatively recent copy of npm, which is covered next.

> Note that Node-API was first supported experimentally in Node 8.0.0. You will need a copy of Node that supports Node-API in order to develop and run Node-API code. To see which versions of Node support Node-API, refer to the [Node-API Version Matrix](https://nodejs.org/api/n-api.html#n_api_node_api_version_matrix).

## npm

[npm](https://www.npmjs.com), the "Node Package Manager", is a set of tools for creating, maintaining, and sharing JavaScript modules. A module is a single set of JavaScript and other files that perform a specific useful purpose. You'll be using modules created by others as well as creating your own modules. npm facilities a vibrant open source community and offers [accounts](https://www.npmjs.com/pricing) that permit individuals and organizations to publish private modules.

For most Node-API users, the objective is to publish an npm module containing their C/C++ code and the Node-API code

npm is typically installed alongside Node. There is more information [here](https://www.npmjs.com/get-npm) about installing npm and keeping it up-to-date.

## Git

Although not strictly required for Node-API work, [Git](https://git-scm.com) is a widely used distributed version control system used in many software development projects. Git is beyond the scope of this site.

## C/C++ and Python

Besides Node and npm, you'll need a set of C and C++ development tools and Python.

### Windows

It is not necessary to install the entire Visual Studio IDE in order to develop and run Node-API modules on Windows. There is an npm module, [`windows-build-tools`](https://www.npmjs.com/package/windows-build-tools), that has everything you need.

Open  [PowerShell](https://docs.microsoft.com/en-us/powershell/scripting/getting-started/getting-started-with-windows-powershell?view=powershell-6) or `cmd.exe` as *Administrator* and enter:

```bash
npm install --global --production windows-build-tools
```

This module also installs Python.

### Mac

Assuming you haven't yet installed Apple's Xcode IDE, the most direct route for installing the C/C++ tools you need for Node-API is to run this command in the Terminal program:

```bash
xcode-select --install
```

If this fails for some reason, you will need to install the complete [Xcode IDE](https://developer.apple.com/xcode/ide/) which will also installs the necessary C/C++ command line tools.

Python comes installed on all versions of macOS prior to macOS Catalina. This command will verify if Python is installed on your system:

```bash
python --version
```

If needed, installers for Python can be found [here](https://www.python.org/downloads/).

### Linux

The necessary C/C++ toolchain and Python are typically already installed on most current Linux distributions. If they are missing on your system, the best way to install the tools is determined by the particular Linux distribution you are using.

Information about installing the LLVM C/C++ toolchain can be found [here](https://llvm.org). Installers for Python can be found [here](https://www.python.org/downloads/).

## Verifying your tools

After installing all of the above tools, each of the commands shown below should return useful information.

### Mac and Linux

```bash
node --version
npm --version
python --version
git --version
cc --version
make --version
```

### Windows

```bash
node --version
npm --version
python --version (TBD)
git --version
cc --version (TBD)
make --version (TBD)
```

## Other tools

You'll need your favorite shell program which typically comes pre-installed on macOS and Linux. For Windows, you may want to consider [PowerShell](https://docs.microsoft.com/en-us/powershell/scripting/getting-started/getting-started-with-windows-powershell?view=powershell-6).

And you'll need your favorite code editor.

There are also powerful IDEs (Integrated Development Environments) that are popular in the developer community. Among these are [Visual Studio Code](https://code.visualstudio.com) and [WebStorm](https://www.jetbrains.com/webstorm/). Depending upon your level of involvement with this work, these tools can quickly pay for themselves in increased productivity.
