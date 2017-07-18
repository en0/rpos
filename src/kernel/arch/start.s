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

        .global _start
        .extern initIDT
        .extern initGDT
        .extern kprintf
        .extern kclear

        .set VALID_MAGIC, 0x2BADB002

        .section .tstack, "aw", @nobits
        .skip 4096              ## 4KB of stack space.
stack:  nop

        .section .bss
mbi:    .long 0x0

        .section .rodata
MSGERR: .asciz "System failed to boot. I dont know why.\n"
MSGHTL: .asciz "System Halted.\n"
MSGFTL: .asciz "Ooh Snap!\n\nYour bootloader does not appear to be multiboot complient.\n"

        .section .text
_start: movl $stack, %esp       ## Setup temp stack
        pushl $0                ## Reset EFLAGS
        popf
        cmp $VALID_MAGIC, %eax  ## Check the multiboot magic number
        je .start

        push $MSGFTL            ## eax was not the expected value.
        call kabort             ## Abort with message

.start: mov %ebx, (mbi)         ## Backup Multiboot Info

        ## Install Descriptor Tables
        call initGDT
        call initIDT

        # Setup the RTC
        mov $0b00110100, %al
        out %al, $0x43

        mov $0x04AA, %ax
        out %al, $0x40
        shr $8, %ax
        out %al, $0x40
        sti
        

        #out %al, $0x40
        #outb $0x04, $0x40

        #outb $0b00110100, $0x43
        #outb $0xAA, $0x40
        #outb $0x04, $0x40

        ## relocate stack
        push mbi
        ##call find_stack
        ##mov %eax, %esp

        call kclear             ## Clear the screen

        push mbi                ## We have a new stack, Repush the mbi
        ##call system_init      ## Call system_init(mboot*)
        call main               ## Call main(mboot*)

        ## Failed Boot
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
