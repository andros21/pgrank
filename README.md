<!-- PROJECT LOGO -->
<br>
<div align="center">
  <a href="https://github.com/andros21/pgrank">
    <img src="https://user-images.githubusercontent.com/58751603/167304963-dc6f6948-ab4f-4e2a-b382-e9f7a2bd9e76.png" alt="Logo" width="300">
  </a>
  <br>
  <br>
  <a href="https://github.com/andros21/pgrank/actions/workflows/meson.yml">
    <img src="https://img.shields.io/github/workflow/status/andros21/pgrank/Meson%20CI/master?label=Meson%20CI&logo=github&style=flat-square" alt="Meson CI">
  </a>
  <a href="https://github.com/andros21/pgrank/actions/workflows/meson.yml">
    <img src="https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/andros21/0e20cd331d0800e3299298a3868aab7a/raw/pgrank__master.json" alt="Coverage">
  </a>
  <a href="https://github.com/andros21/pgrank/actions/workflows/jreleaser.yml">
    <img src="https://img.shields.io/github/workflow/status/andros21/pgrank/JReleaser?label=JReleaser%20CD&logo=github&style=flat-square" alt="JReleaser CD">
  </a>
  <br>
  <a href="https://github.com/andros21/pgrank/releases">
    <img src="https://img.shields.io/github/v/release/andros21/pgrank?color=blue&label=Release&sort=semver&style=flat-square" alt="Version">
  </a>
  <a href="https://github.com/andros21/pgrank/blob/master/LICENSE">
    <img src="https://img.shields.io/github/license/andros21/pgrank?color=blue&label=License&style=flat-square" alt="License">
  </a>

  <h3 align="center">cpp app for computing <a href="https://en.wikipedia.org/wiki/PageRank">pagerank</a></h3>
  <div align="center">
    <a href="examples/caibg/">Examples</a>
    ·
    <a href="#installation">Installation</a>
    ·
    <a href="#usage">Usage</a>
  </div>
</div>

## Installation

### Prerequisites

**Platform requirements**

`Linux x86_64`

**Compiler requirements**

A `C++` compiler (e.g. `g++-9`) supporting the `C++2a` or `C++20` standard

**Build system requirements**

[`Meson`](http://mesonbuild.com) build system version and `Ninja` (its dependency)

Also `CMake` (to build subprojects) and `pkgconfig` (to search library dependencies) are needed

If Meson is not available on your system, it can be installed with `pip` or `conda`

**Dependencies**

`pgrank` requires the following packages to be installed on the system:

* [`armadillo`](http://arma.sourceforge.net) - a high quality linear algebra library (matrix maths) C++-native
* [`OpenBLAS`](https://github.com/xianyi/OpenBLAS) - linked with `armadillo`, multi-threading on intensive tasks
* [`rapidJSON`](https://github.com/Tencent/rapidjson/) - a fast JSON parser and generator for C++
* [`argparse`](https://github.com/p-ranav/argparse) - argument parser for modern C++

`argparse` (header only library), if not available on your system, is automatically built and installed as subprojects with the `Meson` build system, so in this sense are "optional" because provided by `Meson` itself

### Meson build system

To compile and install `pgrank`, run:

```
meson setup \
      --buildtype=release \
      --prefix=/install/path build
cd build/
meson compile
meson install
```

Where `build` is the build directory. Specifying the installation prefix with `--prefix` is optional. On Debian-based systems (e.g. Ubuntu) one may want to use `--libdir=lib`, otherwise libraries will be installed in `$PREFIX/lib/<archdir>` where `<archdir>` is e.g. `x86_64-linux-gnu`

### For developers

For **testing purpose** (developers only) working inside a conda env is probably the best way to build and use this project. Additional dependencies:

* [`catch2`](https://github.com/catchorg/Catch2) - a modern, C++-native, header-only, test framework for unit-tests
* [`gcovr`](https://gcovr.com/en/stable/) - extensions of GCOV, for code coverage

`catch2` (header only library), if not available on your system, are automatically built and installed as subprojects with the `Meson` build system, so in this sense are "optional" because provided by `Meson` itself

To compile, and test `pgrank`, run:

```
meson setup -Dtest=true -Db_coverage=true build
cd build/
meson compile
meson test -v
ninja coverage
```

The target `coverage` will try to create code coverage report in all three formats `text` `xml` `html` (`coverage-html` needs `lcov` to be generated). Optionaly only one coverage format can be triggered (e.g `coverage-text`)

## Usage

You need a special formatted json as input to create the graph (see [`json2mat.hpp`](src/json2mat/json2mat.hpp) header for more informations or inside [`caibg`](examples/caibg)). And stop, all other parameters can be set using cli `pgrank --help`
