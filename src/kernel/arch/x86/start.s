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
        .global kabort

        .extern find_stack
        .extern system_init
        .extern main
        .extern x86_kprintf
        .extern x86_kclear

        ## Multiboot Validation
        .set VALID_MAGIC, 0x2BADB002

        .section .rodata

unknown_error: .asciz "System failed to boot. I dont know why.\n"
halt_message: .asciz "System Halted.\n"
mboot_fail: .asciz "Ooh Snap!\n\nYour bootloader does not appear to be multiboot complient.\n"


        .section .tstack, "aw", @nobits
mbi:    .long 0x0

        .section .text

        ## Kernel Entry 

_start: ## Setup The temp stack
        movl $stack, %esp

        ## Reset EFLAGS
        pushl $0
        popf

        ## Validate boot loader is multiboot complient
        cmp $VALID_MAGIC, %eax
        je .mbcomplient

        ## If we get here then multiboot is not supported
        ## abort with message
        push $mboot_fail
        call kabort

.mbcomplient:

        ## Backup MBI
        mov %ebx, (mbi)

        ## relocate stack
        push mbi
        call find_stack
        mov %eax, %esp

        ## Install flat GDT
        nop

        ## Clear screen for a pretty loader
        call x86_kclear

        push mbi            ## We have a new stack, Repush the mbi
        call system_init    ## Call system_init(mboot*)
        call main           ## Call main(mboot*)

        ## Failed Boot
        push $unknown_error
        call kabort


kabort: push %ebp
        mov %esp, %ebp
        ## call x86_kclear - That is silly - i cannot see debuging logs
        mov 0x8(%ebp), %eax
        push %eax
        call x86_kprintf
        push $halt_message
        call x86_kprintf
        cli
        hlt

        ## Safty loop
.hang:  jmp .hang

        ## Allocate a 16K temporary stack that will get the job done
        ## untill we can get memory management setup.
        .section .tstack, "aw", @nobits
        .skip 512 ## Don't need much room here.
stack:  nop

