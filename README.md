# Overview

This a simple package for converting between time expressed as seconds since the epoch (January 1 1970) and time expressed as year/month/day/hour/minute/second.

# Prerequisites

   1. A C++17 compiler.
   2. [CMake](https://cmake.org/) v.3.16 or greater.
   3. Howard Hinnant's [date](https://github.com/HowardHinnant/date) header-only library.
   4. [Google Test](https://github.com/google/googletest)

# Building and Installing

After the prerequisites are installed the library can be configured with a script, e.g., configure.sh

    #!/bin/bash
    export CXX=clang++
    export BUILD_DIR=clang_build
    if [ -d ${BUILD_DIR} ]; then
       rm -rf ${BUILD_DIR}
    fi
    mkdir ${BUILD_DIR}
    cd ${BUILD_DIR}
    cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_COMPILER=${CXX} \
    -DCMAKE_CXX_FLAGS="-Wall -O2" 

After running 

    configure.sh

you would then descend into the build directory, e.g.,

    cd clang_build
    make
    make test
    make install

Note, the install command may require sudo permissions.
