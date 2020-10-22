# [FABRIK.C++](#) [![Build Status](https://travis-ci.org/polytypic/fabrik.cpp.svg?branch=v1)](https://travis-ci.org/polytypic/fabrik.cpp)

[FABRIK](http://andreasaristidou.com/FABRIK.html) implementation in C++.

See [`synopsis.hpp`](provides/include/fabrik_v1/synopsis.hpp) for the API.

Examples:

- Basic: [page](https://polytypic.github.io/fabrik.cpp/basic.html)
  [code](internals/basic/program/main.cpp)

### Building

This project uses the [C++ submodule manager](https://cppsm.github.io/) and the
browser samples depend on [Emscripten](https://emscripten.org/). If you have the
[`cppsm`](https://cppsm.github.io/#installation) command in path, and the
[`emsdk`](https://emscripten.org/docs/getting_started/downloads.html#installation-instructions)
environment variables, then you can just clone and test this project:

```bash
cppsm clone "git@github.com:polytypic/fabrik.cpp.git" v1
cd fabrik.cpp
CC=emcc cppsm test
```

To test the browser samples you will need to serve the `docs` folder. For
examples, Using
[`npx`](https://medium.com/@maybekatz/introducing-npx-an-npm-package-runner-55f7d4bd282b)
you could say [`npx serve docs`](https://www.npmjs.com/package/serve) and then
point your browser to
[http://localhost:5000/basic](http://localhost:5000/basic).
