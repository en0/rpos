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

MAKE_CONFIG=../../config.mk

include $(MAKE_CONFIG)

#ARCH_PATH := arch
#LIB_PATH := lib
#LIB_PATH_ARCH := $(LIB_PATH)/arch
KERNEL=x86-elf-kernel

#OBJS=$(patsubst %.c, %.c.o, $(wildcard *.c) $(wildcard $(ARCH_PATH)/*.c) $(wildcard $(LIB_PATH)/*.c) $(wildcard $(LIB_PATH_ARCH)/*.c)) \
#     $(patsubst %.s, %.s.o, $(wildcard *.s) $(wildcard $(ARCH_PATH)/*.s) $(wildcard $(LIB_PATH)/*.s) $(wildcard $(LIB_PATH_ARCH)/*.s))

CFILES=$(shell find . -type f -name '*.c')
SFILES=$(shell find . -type f -name '*.s')

OBJS=$(patsubst %.c, %.c.o, CFILES) \
     $(patsubst %.s, %.s.o, SFILES)

CONFIG_H=$(patsubst include/%.h.in, include/%.h, $(wildcard include/*.h.in))

## Determin Profile
ifeq ($(PROFILE), DEBUG)
K_CFLAGS := $(K_CFLAGS) -ggdb
K_ASFLAGS := $(K_ASFLAGS) -ggdb
endif

## BUILD

.PHONY : all

all : $(KERNEL)

$(KERNEL) : $(OBJS)
	@echo \ - LD $@
	@$(K_LD) $(K_LDFLAGS) -T linker.ld -o $@ $^

# Generic Build Targets:
%.c.o : %.c
	@echo \ - CC $@
	@$(K_CC) $(K_CFLAGS) -Iinclude -I$(ARCH_PATH)/include -o $@ -c $<

%.s.o : %.s
	@echo \ - AS $@
	@$(K_AS) $(K_ASFLAGS) -o $@ $<

%.h : %.h.in $(MAKE_CONFIG)
	@echo \ - GENCONFIG $@
	@$(GENCONFIG) $(MAKE_CONFIG) $< $@

# Dependencies
$(OBJS) : $(CONFIG_H)


## CLEAN

CLEAN_OBJS=$(OBJS:%=clean-%) $(KERNEL:%=clean-%) $(CONFIG_H:%=clean-%)

.PHONY : clean clean-notice $(CLEAN_OBJS)

clean : $(CLEAN_OBJS)

$(CLEAN_OBJS) :
	@echo \ - RM $(@:clean-%=%)
	@$(RM) $(@:clean-%=%)


## DISK

.PHONY : disk

disk :
	@echo '** NOT IMPLEMENTED **'


## RUN

.PHONY : run

run : $(KERNEL)
	@echo NOTICE: Profile is $(PROFILE)
	@$(QEMU) -kernel $<


.PHONY : debug

debug : $(KERNEL)
	@echo NOTICE: Profile is $(PROFILE)
	@echo Start debugger and attache to host on port 1234
	@echo Example with GDB:
	@echo \ target remote localhost:1234
	@echo \ symbol-file $<
	@$(QEMU) -s -S -kernel $<


## INFO

.PHONY : info

info :
	@echo ""
	@echo Build configuration information.
	@echo ""
	@echo Build Objects \"\($(PROFILE)\)\":
	@echo $(OBJS) | xargs -n1 | sed s/^/\ -\ /
	@echo ""
	@echo Configuration Headers.
	@echo $(CONFIG_H) | xargs -n1 | sed s/^/\ -\ /
	@echo ""
	@echo Using cross compiler utilities with the respective flags:
	@echo \ - CC : $(K_CC)
	@echo \ - CFLAGS : $(K_CFLAGS)
	@echo ""
	@echo \ - LD : $(K_LD)
	@echo \ - LDFLAGS : $(K_LDFLAGS)
	@echo ""
	@echo \ - AS : $(K_AS)
	@echo \ - ASFLAGS : $(K_ASFLAGS)
	@echo ""
	@echo \ - AR : $(K_AR)
	@echo ""

