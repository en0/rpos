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

## REGION: GLOBAL

# Project Settings
PROJNAME = RPOS
PROFILE = DEBUG

# Project root path
TOP := $(dir $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

# Top Level Build targets
SUBDIRS = src/kernel ramdisk

# Some Utilities
GENCONFIG=$(TOP)/scripts/genconfig.py
#QEMU=qemu-system-x86_64 -m 128 -serial stdio
QEMU=qemu-system-x86_64 -m 128 -serial stdio -initrd ../../ramdisk/initrd.img

## END REGION: GLOBAL


## REGION: KERNEL

# Cross Compiler Settings
K_CC=i686-elf-gcc
K_LD=i686-elf-ld
K_AS=i686-elf-as
K_AR=i686-elf-ar

# FLAGS
K_CFLAGS = -ffreestanding -O0 -Wall -nostdlib -fno-stack-protector -fno-omit-frame-pointer
K_ASFLAGS=
K_LDFLAGS=

## END REGION: KERNEL

