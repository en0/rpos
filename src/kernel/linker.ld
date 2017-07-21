/**
 ** Copyright (c) 2017 "Ian Laird"
 ** Research Project Operating System (rpos) - https://github.com/en0/rpos
 ** 
 ** This file is part of rpos
 ** 
 ** rpos is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 ** 
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 ** 
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

ENTRY(ldr)
SECTIONS
{

    /* Begin putting sections at 1 MiB, a conventional place for kernels to be
     * loaded at by the bootloader. */

    . = 0xC0100000;

    PROVIDE(_kernel = .);

    /* First put the multiboot header, as it is required to be put very early
     * early in the image or the bootloader won't recognize the file format.
     * Next we'll put the .text section. */

    .text : AT(ADDR(.text) - 0xC0000000) {
        *(.multiboot)
        *(.text)
        *(.rodata)
    }

    /* Read-write data (initialized) */

    .data ALIGN(4K) : AT(ADDR(.data) - 0xC0000000) {
        *(.data)
    }

    /* Read-write data (uninitialized) and stack */

    .bss : AT(ADDR(.bss) - 0xC0000000) {
        *(COMMON)
        *(.bss)

        /* This stack refrence is temporary. We will exclude it from the locked memory because
         * we will be setting up a new stack in the pmemory manager and we want to reclaime ths
         * space. That is why kernel end does not take the tstack region into account. */

        *(.tstack)
    }

    .end = .;
    PROVIDE(_ekernel = .end);
}
