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


include config.mk


## BUILD ALL TARGETS (make all)

.PHONY : all $(SUBDIRS)

all : $(SUBDIRS)

$(SUBDIRS) : 
	@$(MAKE) -C $@


## CLEAN ALL TARGETS (make clean)

CLEANDIRS = $(SUBDIRS:%=clean-%)

.PHONY : clean $(CLEANDIRS)

clean : $(CLEANDIRS)

$(CLEANDIRS) :
	@$(MAKE) -C $(@:clean-%=%) clean


## DISK ALL TARGETS (make disk)

DISKDIRS = $(SUBDIRS:%=disk-%)

.PHONY : disk $(DISKDIRS)

disk : $(DISKDIRS)

$(DISKDIRS) :
	@$(MAKE) -C $(@:disk-%=%) disk


## RUN SPECIFIC TARGET (make run-[target])

RUNDIRS = $(SUBDIRS:%=run-%)

.PHONY : $(RUNDIRS)

$(RUNDIRS) :
	@$(MAKE) -C $(@:run-%=%) run


## DEBUG SPECIFIC TARGET (make debug-[target])

DBGDIRS = $(SUBDIRS:%=debug-%)

.PHONY : $(DBGDIRS)

$(DBGDIRS) :
	@$(MAKE) -C $(@:debug-%=%) debug


## INFO ALL TARGETS (make info)

INFODIRS = $(SUBDIRS:%=info-%)

.PHONY : info $(INFODIRS)

info : $(INFODIRS)

$(INFODIRS) :
	@$(MAKE) -C $(@:info-%=%) info


## Help/Options list

.PHONY : help options

help : options

options : 
	@echo ""
	@echo $(PROJNAME) Build System Options
	@echo ---------------------------------------
	@echo Build Targets: Compile the provided target.
	@echo all $(SUBDIRS) | xargs -n1 | sed s/^/\ -\ make\ /
	@echo ""
	@echo clean Targets: Remove all files generated by Build Targets.
	@echo clean $(CLEANDIRS) | xargs -n1 | sed s/^/\ -\ make\ /
	@echo ""
	@echo Disk Targets: Install the targets into a virtual disk image.
	@echo disk $(DISKDIRS) | xargs -n1 | sed s/^/\ -\ make\ /
	@echo ""
	@echo Run Targets: Run a specific target if supported.
	@echo $(RUNDIRS) | xargs -n1 | sed s/^/\ -\ make\ /
	@echo ""
	@echo Debug Targets: Run a specific target with debugger if supported.
	@echo $(DBGDIRS) | xargs -n1 | sed s/^/\ -\ make\ /
	@echo ""
	@echo Info Targets: Display build info for the privided target.
	@echo info $(INFODIRS) | xargs -n1 | sed s/^/\ -\ make\ /
	@echo ""

