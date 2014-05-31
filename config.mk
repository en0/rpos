## REGION: GLOBAL

# Project Settings
PROJNAME = RPOS
PROFILE = DEBUG
ARCH=x86


# Project root path
TOP := $(dir $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

# Top Level Build targets
SUBDIRS = src/kernel

# Some Utilities
GENCONFIG=$(TOP)/scripts/genconfig.py
QEMU=qemu-system-x86_64

## END REGION: GLOBAL


## REGION: KERNEL

# Cross Compiler Settings
K_CC=i686-elf-gcc
K_LD=i686-elf-ld
K_AS=i686-elf-as
K_AR=i686-elf-ar

# FLAGS
K_CFLAGS=
K_ASFLAGS=
K_LDFLAGS=

## END REGION: KERNEL

