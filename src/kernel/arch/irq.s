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

                .extern irq_dispatch

                .section .text
                .align 4

.macro irq_stub num
                .global irq_stub\num
irq_stub\num:   cli                     # Disable interrupts.
                pusha                   # Backup all GP Registers
                mov %esp, %ebp          # Backup the stack ponter
                push $\num              # Push the irq number onto the stack.
                jmp irq_common          # goto handler stub
.endm

irq_common:     call irq_dispatch       # Call the vector dispatch in irq.c
                mov %ebp, %esp          # Restore stack pointer
                popa                    # Restore all GP registers
                sti                     # Turn on interrupts.
                iret                    # Go back to what we where doing.

                irq_stub 0
                irq_stub 1
                irq_stub 2
                irq_stub 3
                irq_stub 4
                irq_stub 5
                irq_stub 6
                irq_stub 7
                irq_stub 8
                irq_stub 9
                irq_stub 10
                irq_stub 11
                irq_stub 12
                irq_stub 13
                irq_stub 14
                irq_stub 15
