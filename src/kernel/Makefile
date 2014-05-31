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


include ../../config.mk

ARCH_PATH := arch/$(K_TARGET)
KERNEL=$(K_TARGET)-elf-kernel

OBJS=$(patsubst %.c, %.c.o, $(wildcard *.c)) \
     $(patsubst %.s, %.s.o, $(wildcard *.s)) \
     $(patsubst $(ARCH_PATH)/%.c, $(ARCH_PATH)/%.c.o, $(wildcard $(ARCH_PATH)/*.c)) \
     $(patsubst $(ARCH_PATH)/%.s, $(ARCH_PATH)/%.s.o, $(wildcard $(ARCH_PATH)/*.s))


## BUILD

.PHONY : all

all : 
	@echo '** NOT IMPLEMENTED **'

# Generic Build Targets:
%.c.o : %.c
	$(CC) $(K_CFLAGS) -o $@ $<

%.s.o : %.s
	$(AS) $(K_ASFLAGS) -o $@ $<


## CLEAN

CLEAN_OBJS=$(OBJS:%=clean-%) $(KERNEL:%=clean-%)

.PHONY : clean clean-notice $(CLEAN_OBJS)

clean : $(CLEAN_OBJS)

$(CLEAN_OBJS) : clean-notice
	@echo \ - Removing Target: $(@:clean-%=%)
	@$(RM) $(@:clean-%=%)

clean-notice :
	@echo Cleaning build objects...


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
	@echo Build configuration information for all targets.
	@echo ""
	@echo Objects for target \"$(K_TARGET)\":
	@echo $(OBJS) | xargs -n1 | sed s/^/\ -\ /
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

