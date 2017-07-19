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

            .global initIDT
            .global idt_setGate

            .section .bss
            .align 4

idt_start:  .skip 2048
idt_end:

            .section .rodata
            .align 4

idt_info:   .2byte idt_end - idt_start - 1
            .4byte idt_start

            .section .text
            .align 4

initIDT:    lidt idt_info       # Install the new IDT
            ret                 # Head back to start.s

idt_setGate:push %ebp
            mov %esp, %ebp

            mov $idt_start, %eax
            mov 0x8(%ebp), %edx     # Param: uint32_t index
            lea (%eax,%edx,8), %edx # edx = idt_start + (index * 8)

            mov 0xC(%ebp), %eax     # uint32_t address
            mov 0x10(%ebp), %ax     # Load type attributes
            mov %eax, 0x04(%edx)    # Write the second word in the IDT Entry

            mov $0x0008, %eax       # GDT Segment selector (always 0x08)
            mov 0xC(%ebp), %ebx     # uint32_t address
            shl $16, %eax           # Mov the segment selector to the top half of eax
            mov %bx, %ax            # set the lower 16 bits of the address.
            mov %eax, (%edx)        # Write the first word in the IDT entry

            mov %ebp, %esp
            pop %ebp
            ret
