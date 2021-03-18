---
id: about.uses
title: Uses for Node-API
prev: about.what
---

## An existing C/C++ library

Perhaps the most popular use for Node-API is making the capabilities of an existing C/C++ library available to JavaScript programmers. This permits you to leverage the investment you've already made in your existing code, making it available to a whole new population of JavaScript programmers and projects. 

For many applications, the C/C++ code base is the reference implementation. Node-API permits you to continue to refine and maintain your existing C/C++ code. Improvements to the C/C++ code are then easily transferred to the JavaScript users.  

## Access to OS resources

Some applications, like those built on [Electron](https://electronjs.org) or [NW.js](https://nwjs.io), can benefit from accessing system toolkits and APIs not currently available through Node. Node-API facilities accessing these system resources. 

## Computational tasks

For low-powered devices, it may make sense to code computationally intensive tasks in C or C++ and then make that code available to JavaScript using Node-API. Although in many cases Node's JavaScript runtime engine will eventually compile the JavaScript down to binary, Node-API offers the ability to compile the C/C++ code just once and have the binary available to Node right from the start. 