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

        # This file defines the memory where the IDT will reside and had a
        # install method that can be called from C or start.s. It is expected
        # that paging is enabled and linear address are used (paging matters)
        # e.g. install the gdt after the kernel is in higher half.

            .global initIDT

            ## Decsriptor layout
            ## //.2byte offset_1     # Offset1 0..15
            ## //.2byte 0x0008       # Segment Selector
            ## //.byte  0x00         # Reserved
            ## //.byte  0b10001110   # Type Attr (Present and 32-bit Int Gate
            ## //.2byte offset_2     # offset2 16..31

            .macro install_idt_entry entry offset
                mov $\offset, %eax
                mov $0x8E00, %ax
                mov %eax, \entry+4
                mov $0x0008, %ax
                mov $\offset, %ebx
                shl $16, %eax
                mov %bx, %ax
                mov %eax, \entry
            .endm

            .section .bss
            .align 4

idt_start:  .skip 2048
idt_end:

            .section .rodata
            .align 4

msg: .asciz "\n[x] Hello, from IRQ! %p\n"

idt_info:   .2byte idt_end - idt_start - 1
            .4byte idt_start

            .section .text
            .align 4

        # installIDT can be called AFTER the idt has been loaded.

initIDT:    push %ebp           # Backup the stack frame
            mov %esp, %ebp

            push %eax           # Save eax and ebx
            push %ebx           # so we don't clobber them

            .set idt_start_offset, 0x00
            .rept 255
            install_idt_entry idt_start+idt_start_offset irq0
            .set idt_start_offset, idt_start_offset+0x08
            .endr

            lidt idt_info       # Install the new IDT
            #sti                 # Enable Interrupts! probably want to remap the PIC first

break:      pop %ebx            # Restore eax and ebx
            pop %eax            # so they have all the multi boot stuff
            mov %ebp, %esp      # Restore the stack frame
            pop %ebp
            ret                 # Head back to start.s

        # Temp IRQ Hander that prints a message to the screen when it is
        # called. Just testing things out.

irq0:       cli
            push %ebp
            mov %esp, %ebp
            sub $0x1c, %esp

            #movl $0xDEADBEEF, 0x4(%esp)
            movl %eax, 0x4(%esp)
            movl $msg, (%esp)
            call kprintf

            mov %ebp, %esp
            pop %ebp

            sti
            iret
