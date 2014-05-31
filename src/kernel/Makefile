## Copyright (c) 2014 "Ian Laird"
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

ARCH_PATH := arch/$(K_TARGET)
KERNEL=$(K_TARGET)-elf-kernel

OBJS=$(patsubst %.c, %.c.o, $(wildcard *.c)) \
     $(patsubst %.s, %.s.o, $(wildcard *.s)) \
     $(patsubst $(ARCH_PATH)/%.c, $(ARCH_PATH)/%.c.o, $(wildcard $(ARCH_PATH)/*.c)) \
     $(patsubst $(ARCH_PATH)/%.s, $(ARCH_PATH)/%.s.o, $(wildcard $(ARCH_PATH)/*.s)) \

CONFIG_H=$(patsubst include/%.h.in, include/%.h, $(wildcard include/*.h.in))

## BUILD

.PHONY : all

all : $(KERNEL)

$(KERNEL) : $(OBJS)
	@echo \ - LD $@
	@$(K_LD) $(K_LDFLAGS) -o $@ $^

# Generic Build Targets:
%.c.o : %.c
	@echo \ - CC $@
	@$(K_CC) $(K_CFLAGS) -o $@ -c $<

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

run :
	@echo '** NOT IMPLEMENTED **'


.PHONY : debug

debug :
	@echo '** NOT IMPLEMENTED **'


## INFO

.PHONY : info

info :
	@echo ""
	@echo Build configuration information.
	@echo ""
	@echo Objects for target \"$(K_TARGET)\":
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

