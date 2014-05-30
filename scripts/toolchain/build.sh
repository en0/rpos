#!/bin/env bash

. config.sh

echo ""
echo "Please review config.sh before running!"
echo ""
echo "Will remove existing build directories if they exist"
read -p "To continue press [ENTER]. Press [ctrl+c] to cancle"

BUILD_BINUTILS="build-bu"
BUILD_GCC="build-gcc"

rm -rf $BUILD_BINUTILS
rm -rf $BUILD_GCC

echo "Building BINUTILS with the following flags: $BINUTILS_FLAGS"
echo "Building GCC with the following flags: $GCC_FLAGS"
read -p "To continue press [ENTER]. Press [ctrl+c] to cancle"

# Build bin utils
mkdir $BUILD_BINUTILS
cd $BUILD_BINUTILS
../binutils-*.*/configure $BINUTILS_FLAGS
make
make install
cd ..

# Build gcc
mkdir $BUILD_GCC
cd $BUILD_GCC
../gcc-*.*.*/configure $GCC_FLAGS
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
cd ..

