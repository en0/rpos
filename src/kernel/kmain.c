/**
 ** Copyright (c) 2014 "Ian Laird"
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

#include <kernel.h>
#include <multiboot.h>
#include <kprint.h>
#include <string.h>

void main(multiboot_info_t* bootinfo) {
    /* bootinfo points to the multiboot header
     * end_of_kernel points to the bottom of the stack
     * We need to lock memory to end_of_kernel and set up a new stack */

    kprintf("Research Porject Kernel\n"
            "Kernel starts at %p\n"
            "Kernel ends at %p\n"
            "Paging is enabled!\n",
            &_start, &_end);

    // Create a new page directory (Like for user space or something)
    vmem_context *s = vmem_copy_context(NULL);

    // Allocate some memory and a specific address
    char* x = (char*)vmem_kalloc(
        s, (virt_addr*)0x500000,
        4096, x86_FLG_VMEM_WRITABLE );

    kprintf("Allocated space under page table %i=>%p\n", s, x);

    // Switch to the new paging table.
    vmem_activate(s);

    strncat(x, "Hello, world!", 13);
    kprintf("VALUE: %s\n", x);

    for(;;);
}

