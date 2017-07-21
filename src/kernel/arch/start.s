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

        .set VALID_MAGIC, 0x2BADB002

        .global _start
        .global kabort
        .extern initIDT
        .extern initGDT
        .extern initDBG
        .extern kprintf
        .extern kclear


        .section .tstack, "aw", @nobits

        .skip 4096              ## 4KB of stack space.
stack:  nop


        .section .rodata

MSGERR: .asciz "System failed to boot. I dont know why.\n"
MSGHTL: .asciz "System Halted.\n"
MSGFTL: .asciz "Ooh Snap!\n\nYour bootloader does not appear to be multiboot complient.\n"


        .section .text
        .align 4

_start: movl $stack, %esp       ## Setup temp stack
        pushl $0                ## Reset EFLAGS
        popf
        cmp $VALID_MAGIC, %eax  ## Check the multiboot magic number
        je .start

        push $MSGFTL            ## eax was not the expected value.
        call kabort             ## Abort with message

.start: push %ebx               ## EBX points to multiboot_info_t
        call kclear             ## Clear the screen
        call system_init        ## Call system_init(mboot*)
        call main               ## Call main(mboot*)

        ## This should never happen
        push $MSGERR
        call kabort

kabort: push %ebp
        mov %esp, %ebp
        mov 0x8(%ebp), %eax
        push %eax
        call kprintf
        push $MSGHTL
        call kprintf
        cli
        hlt

        ## Safty loop
.hang:  jmp .hang
