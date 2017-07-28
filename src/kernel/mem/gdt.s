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

            .global gdt_setup

            .section .data

        # This GDT defines a flat segmentation model. 1 for code, 1 for data
        # both for ring 0 and ring 3. There is one additional space to set the
        # TSS up later. REF: http://wiki.osdev.org/Global_Descriptor_Table
        # BBBBBBBBFFFFLLLLAAAAAAAAbbbbbbbbbbbbbbbbbbbbbbbbllllllllllllllll

            # Null Descriptor
gdt_start:  .8byte 0

            # For kernel space
.gdt08:     .8byte 0b0000000011001111100110000000000000000000000000001111111111111111
.gdt10:     .8byte 0b0000000011001111100100100000000000000000000000001111111111111111

            # For user space
.gdt18:     .8byte 0b0000000011111111100110000000000000000000000000001111111111111111
.gdt20:     .8byte 0b0000000011111111100100100000000000000000000000001111111111111111

            # For TSS
.gdt28:     .8byte 0

        # The GDT Descriptor contains the size of the GDT, in bytes minus one,
        # and the linear address (paging applies) to the first gdt entry.

gdt_info:   .2byte gdt_info - gdt_start - 1
            .4byte gdt_start

        # Install the GDT and jump to the new code segment. Then install the
        # new data segment. This should be done AFTER paging is enabled and we
        # are in the higher half. e.g. The higher half addres should be used.

            .section .text
            .align 4

gdt_setup:  push %ebp
            mov %esp, %ebp
            push %eax

loadGDT:    lgdt gdt_info
            jmp $0x08, $loadCS

loadCS:     mov $0x10, %ax
            mov %ax, %ds
            mov %ax, %es
            mov %ax, %fs
            mov %ax, %gs
            mov %ax, %ss

            pop %eax
            mov %ebp, %esp
            pop %ebp

            ret
