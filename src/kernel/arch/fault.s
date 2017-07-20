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

                #.extern fault_trap
                .extern fault_handler

                .section .text
                .align 4

                .macro fault_stub num
                .global fault_stub\num
fault_stub\num: cli                     # Disable interrupts.
                push $0x0               # Push a empty errorCode
                push $\num              # Push the fault number onto the stack.
                jmp fault_common        # goto handler stub
                .endm

                .macro fault_stubc num
                .global fault_stub\num
fault_stub\num: cli                     # Disable interrupts.
                push $\num              # Push the fault number onto the stack.
                jmp fault_common        # goto handler stub
                .endm

fault_common:   
    pusha
    push %ds
    push %es
    push %fs
    push %gs
    mov $0x10, %ax   # Load the Kernel Data Segment descriptor!
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %esp, %eax   # Push us the stack
    push %eax
    mov $fault_handler, %eax
    call *%eax       # A special call, preserves the 'eip' register
    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add 8, %esp     # Cleans up the pushed error code and pushed ISR number
    iret           # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

                fault_stub 0
                fault_stub 1
                fault_stub 2
                fault_stub 3
                fault_stub 4
                fault_stub 5
                fault_stub 6
                fault_stub 7
                fault_stubc 8
                fault_stubc 10
                fault_stubc 11
                fault_stubc 12
                fault_stubc 13
                fault_stubc 14
                fault_stub 16
                fault_stubc 17
                fault_stub 18
                fault_stub 19
                fault_stub 20
                fault_stubc 30
