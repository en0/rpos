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

    char *val = (char*)0x500000;

    // Allocate some kernel space and set some data.
    vmem_kalloc( NULL, (virt_addr*)val, 4096, x86_FLG_VMEM_WRITABLE ); 
    strncat(val, "Hello, Kernel", 13);

    // Allocate a new page and add some data.
    vmem_context *s = vmem_copy_context(NULL);
    vmem_kalloc( s, (virt_addr*)val, 4096, x86_FLG_VMEM_WRITABLE ); 
    vmem_activate(s);
    strncat(val, "Hello, user! ", 13);

    // Switch back to kernel page and show data at 0x500000
    vmem_activate(NULL);
    kprintf("VALUE: %s\n", val);

    // Switch back to new page and show data at 0x500000
    vmem_activate(NULL);
    kprintf("VALUE: %s\n", val);

    for(;;);
}

