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

#include <kernel.h>
#include <multiboot.h>
#include <kprint.h>
#include <string.h>
#include <cpu.h>

void main(multiboot_info_t* bootinfo) {

    int i = 0;
    for(i = 0; i < 80; i++, kputchar('='));
    kputs(  "                       _/_/_/    _/_/_/      _/_/      _/_/_/   \n"
            "                      _/    _/  _/    _/  _/    _/  _/          \n"
            "                     _/_/_/    _/_/_/    _/    _/    _/_/       \n"
            "                    _/    _/  _/        _/    _/        _/      \n"
            "                   _/    _/  _/          _/_/    _/_/_/         ");
    for(i = 0; i < 80; i++, kputchar('='));
                                             
  
    kprintf("[-] Kernel starts at %p\n"
            "[-] Kernel ends at %p\n"
            "[-] Kernel space size is %i KB\n",
            MMAP_KERNEL, MMAP_EKERNEL, 
            (MMAP_EKERNEL - MMAP_KERNEL) / 1024);

    /* Cause a divide by zero fault
    for(i = 3; i > -1; i--)
        kprintf("100 / %i = %i\n", i, 100/i);
    */

    for(;;);
}
