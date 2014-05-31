#!/bin/env bash

TOOLCHAIN_CROSS=$(dirname $(readlink -f $(find ~ -iname i686-elf-gcc | head -n1)))
if [ -z "$TOOLCHAIN_CROSS" ]; then
    echo "CRITICAL: i686-elf-gcc was not found."
    exit 1
fi

echo "NOTICE: Adding to path: ${TOOLCHAIN_CROSS}"
export PATH=$TOOLCHAIN_CROSS:$PATH

