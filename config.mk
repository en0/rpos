## REGION: GLOBAL

# Project Name
PROJNAME = RPOS

# Project root path
TOP := $(dir $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

# Identify the build targets
OUT=$(TOP)build/

# Top Level Build targets
SUBDIRS = src/kernel

## END REGION: GLOBAL


## REGION: KERNEL

# Cross Compiler Settings
K_CC=i686-elf-gcc
K_LD=i686-elf-ld
K_AS=i686-elf-as
K_AR=i686-elf-ar

# Targets
K_TARGET=x86

# FLAGS
K_CFLAGS=
K_ASFLAGS=
K_LDFLAGS=

## END REGION: KERNEL

