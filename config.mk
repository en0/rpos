## REGION: GLOBAL

TOP := $(dir $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

# Identify the build targets
OUT=$(TOP)build/

# Includes
INCLUDE=$(TOP)include

# Libs
LIBS=$(TOP)libs

## END REGION: GLOBAL


## REGION: KERNEL

# Cross Compiler Settings
K_CC=i686-elf-gcc
K_LD=i686-elf-ld
K_AS=i686-elf-as
K_AR=i686-elf-ar

# Targets
K_TARGET=x86

## END REGION: KERNEL

