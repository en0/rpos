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

        .global _start

        .extern main
        .extern _KERNEL_END_

        ## Multiboot Validation
        .set VALID_MAGIC, 0x2BADB002

        .section .text

        ## Kernel Entry 

_start: ## Validate boot loader is multiboot complient
        cmp $VALID_MAGIC, %eax
        jne _hlt

        ## Setup The temp stack
        movl $stack, %esp

        ## Install flat GDT
        nop

        ## Call kmain(mboot*, kernel_end*)
        push _KERNEL_END_
        push %ebx
        call main

        ## Safty loop
_hlt:   cli
        hlt
_hang:  jmp _hang

        ## Allocate a 16K temporary stack that will get the job done
        ## untill we can get memory management setup.
        .section .tstack, "aw", @nobits
        .skip 16384
stack:  nop
