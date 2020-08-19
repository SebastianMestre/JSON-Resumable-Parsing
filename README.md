# Resumable Parsing

A parser for (a subset of) JSON, to which you can feed a character at a time.

This repository contains a small command line utility that takes minified JSON and pretty-prints it.

You can also use the parser as a header-only library, by `#include`ing `parser.hpp` in your project.

## Build

run this command:

```
make -C ./src
```

Note: you will need to have `make` installed on your system, as well as a C++14 (or later) compiler and standard library.

