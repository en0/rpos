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
            #.align 4

idt_start:  .skip 2048
idt_end:

            .section .rodata
            .align 4

msg_sr:     .asciz "\n[x] Hello, from IRQ! %p"
msg_irq:    .asciz "."

idt_info:   .2byte idt_end - idt_start - 1
            .4byte idt_start

            .section .text
            .align 4

        # installIDT can be called AFTER the idt has been loaded.

initIDT:    push %ebp           # Backup the stack frame
            mov %esp, %ebp

            push %eax           # Save eax and ebx
            push %ebx           # so we don't clobber them

            # install irq0 for the RTC
            .set idt_start_offset, 0x00
            install_idt_entry idt_start+idt_start_offset irq0
            .set idt_start_offset, idt_start_offset+0x08
            install_idt_entry idt_start+idt_start_offset irq0
            .set idt_start_offset, idt_start_offset+0x08
            install_idt_entry idt_start+idt_start_offset irq0
            .set idt_start_offset, idt_start_offset+0x08
            install_idt_entry idt_start+idt_start_offset irq0
            .set idt_start_offset, idt_start_offset+0x08
            install_idt_entry idt_start+idt_start_offset irq0
            .set idt_start_offset, idt_start_offset+0x08
            install_idt_entry idt_start+idt_start_offset irq0
            .set idt_start_offset, idt_start_offset+0x08
            install_idt_entry idt_start+idt_start_offset irq0
            .set idt_start_offset, idt_start_offset+0x08
            install_idt_entry idt_start+idt_start_offset irq0
            .set idt_start_offset, idt_start_offset+0x08
            install_idt_entry idt_start+idt_start_offset irq8

            ## These are all broken because they only respond to pic0
            .set idt_start_offset, idt_start_offset+0x08
            install_idt_entry idt_start+idt_start_offset irq0
            .set idt_start_offset, idt_start_offset+0x08
            install_idt_entry idt_start+idt_start_offset irq0
            .set idt_start_offset, idt_start_offset+0x08
            install_idt_entry idt_start+idt_start_offset irq0
            .set idt_start_offset, idt_start_offset+0x08
            install_idt_entry idt_start+idt_start_offset irq0
            .set idt_start_offset, idt_start_offset+0x08
            install_idt_entry idt_start+idt_start_offset irq0
            .set idt_start_offset, idt_start_offset+0x08
            install_idt_entry idt_start+idt_start_offset irq0
            .set idt_start_offset, idt_start_offset+0x08
            install_idt_entry idt_start+idt_start_offset irq0
            .set idt_start_offset, idt_start_offset+0x08
            install_idt_entry idt_start+idt_start_offset irq0
            .set idt_start_offset, idt_start_offset+0x08

            # Mapping everything else to Service Request Zero for now
            .rept 255-16
            install_idt_entry idt_start+idt_start_offset sr0
            .set idt_start_offset, idt_start_offset+0x08
            .endr

            lidt idt_info       # Install the new IDT

break:      pop %ebx            # Restore eax and ebx
            pop %eax            # so they have all the multi boot stuff
            mov %ebp, %esp      # Restore the stack frame
            pop %ebp
            ret                 # Head back to start.s

        # Temp IRQ Hander that prints a message to the screen when it is
        # called. Just testing things out.

sr0:    pusha           # Backup all registers.
        mov %esp, %ebp  # Backup the stack pointer

        push %eax
        push $msg_sr
        call kprintf    # kprintf(msg_sr, %eax)

        mov %ebp, %esp  # Restore stack pointer
        popa            # Restore all registers.
        iret            # Return from interrupt (resets eflags and what not.)

irq0:   cli             # Disable interrupts to prevent timer issues.
        pusha           # Backup all registers.
        mov %esp, %ebp  # Backup the stack pointer

        ## DO SOMETHING

        mov %ebp, %esp  # Restore stack pointer
        popa            # Restore all registers

        mov $0x20, %al
        out %al, $0x20  # send EOI to PIC0

        sti             # Re-enable interrupts
        iret            # Return from interrupt (resets eflags and what not.)

irq8:   cli             # Disable interrupts to prevent timer issues.
        pusha           # Backup all registers.
        mov %esp, %ebp  # Backup the stack pointer

        push $msg_irq
        call kprintf    # kprintf(msg_irq);

        mov %ebp, %esp  # Restore stack pointer
        popa            # Restore all registers

        mov $0x20, %al
        out %al, $0x20  # send EOI to PIC0

        sti             # Re-enable interrupts
        iret            # Return from interrupt (resets eflags and what not.)
