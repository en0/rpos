#!/bin/env bash

## Recomended Folder Structure:
## ~
## + toolchains
##   + i686
##     + cross
##       + src
##         + build.sh
##         + config.sh
##         + tarballs
##           + download.lst
##
## Download the tarballs listed in download.lst
##  While in the cross/src/tarballs directory
##  $ wget -i download.lst
##
## Build the toolchain to install in the cross directory
##  while in the cross/src directory
##  $ ./build.sh
## 

export PREFIX="$(readlink -f $(pwd)/../)"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

export BINUTILS_FLAGS="--target=$TARGET --prefix=$PREFIX --disable-nls --disable-werror"
export GCC_FLAGS="--target=$TARGET --prefix=$PREFIX --disable-nls --enable-languages=c --without-headers"

