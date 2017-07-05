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
            "Kernel ends at %p\n",
            &_start, &_end);

    kprintf("HOLY SHIT - Paging!");
    /*
    kprintf("Attempting to allocate some memory...\n");
    void* m = pmem_alloc();
    char* x = (char*)m;
    memset(m, 0x00, 4096);

    strncat(x, "Hello Everyone!", 5);
    strncat(x, " ", 15);
    strcat(x, "World");
    strcat(x, "!");
    strcat(x, "\n");
    //memcpy(m, "Hello, World!\0", 15);

    if(m == NULL) kprintf("Didnt work\n");
    else {
        kprintf("Data stored in [%p]: %s", m, x);
    }
    */

    for(;;);
}

