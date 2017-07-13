#!/bin/env bash

## Copyright (c) 2017 "Ian Laird"
## Research Project Operating System (rpos) - https://github.com/en0/rpos
## 
## This file is part of rpos
## 
## rpos is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.

TOOLCHAIN_CROSS=$(dirname $(readlink -f $(find ~ -iname i686-elf-gcc | head -n1)))
if [ -z "$TOOLCHAIN_CROSS" ]; then
    echo "CRITICAL: i686-elf-gcc was not found."
    exit 1
fi

echo "NOTICE: Adding to path: ${TOOLCHAIN_CROSS}"
export PATH=$TOOLCHAIN_CROSS:$PATH

