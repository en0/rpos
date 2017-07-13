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

        .global ldr
        .global LDRPDT
        .extern entry

        .equ vBase, 0xC0000000
        .equ ldr, _ldr - vBase
        .equ page_number, (vBase >> 22)

        .section .data
        .align 0x1000

LDRPDT: .4byte 0x83
        .rept page_number - 1
            .4byte 0x80
        .endr

        .4byte 0x83
        .rept 1024 - page_number - 1
            .4byte 0x80
        .endr

        .section .text
        .align 4

        .equ STACKSIZE, 0x4000

_ldr:   mov $(LDRPDT - vBase), %ecx
        mov %ecx, %cr3

        # cr4 |= PSE ; Enable physical size extention.
        mov %cr4, %ecx
        or $0x00000010, %ecx
        mov %ecx, %cr4

        # cr0 |= (PE | PG) ; set protected mode and enable paging.
        mov %cr0, %ecx
        or $0x80000001, %ecx 
        mov %ecx, %cr0

        lea entry, %ecx
        jmp *%ecx
        hlt
