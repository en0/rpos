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

    ## Multiboot Flags

    .set ALIGN, 1<<0
    .set MEMINFO, 1<<1
    ##.set VIDEOINFO, 1<<2 ## Not yet

    ## Compute Multiboot header values

    .set FLAGS, ALIGN | MEMINFO
    .set MAGIC, 0x1BADB002
    .set CHECKSUM, -(MAGIC + FLAGS)

    .section .multiboot

    ## Multiboot Header

    .align 4
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

