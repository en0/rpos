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

#include <string.h>
#include <kernel.h>
#include <mem/vmem.h>


typedef struct
{
    unsigned char name[100];
    unsigned char mode[8];
    unsigned char uid[8];
    unsigned char gid[8];
    unsigned char size[12];
    unsigned char mtime[12];
    unsigned char checksum[8];
    unsigned char type[1];
    unsigned char linkname[100];
    unsigned char tar_indicator[6];
    unsigned char tar_version[2];
    unsigned char owner[32];
    unsigned char group[32];
    unsigned char device_major[8];
    unsigned char device_minor[8];
    unsigned char prefix[155];
}__attribute__((packed)) tar_header_t;

tar_header_t *nodes[5];

unsigned int getsize(unsigned char *in)
{
 
    unsigned int size = 0;
    unsigned int j;
    unsigned int count = 1;
 
    for (j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);
 
    return size;
 
}

typedef struct elf_section_header {
    uint32_t type, p_offset, p_vaddr, p_paddr;
    uint32_t p_filesz, p_memsz;
    uint32_t flags, align;
}__attribute__((packed)) elf_section_header_t;

void main() {

    int i = 0;
    for(i = 0; i < 80; i++, kputchar('='));
    kputs(  "                       _/_/_/    _/_/_/      _/_/      _/_/_/   \n"
            "                      _/    _/  _/    _/  _/    _/  _/          \n"
            "                     _/_/_/    _/_/_/    _/    _/    _/_/       \n"
            "                    _/    _/  _/        _/    _/        _/      \n"
            "                   _/    _/  _/          _/_/    _/_/_/         ");
    for(i = 0; i < 80; i++, kputchar('='));
                                             
    kprintf("[-] Kernel starts at %p\n", VIRT_ADDR_KSTART);
    kprintf("[-] Kernel ends at %p\n", VIRT_ADDR_KEND);
    kprintf("[-] Kernel size is %i KB\n", KERNEL_SIZE / 1024);
    kprintf("[-] Ramdisk at %p\n", VIRT_ADDR_RAMDISK);
    kprintf("[-] Kernel stack at %p (8MB)\n", VIRT_ADDR_STACK);
    kprintf("[-] Heap located at %p\n", VIRT_ADDR_HEAP);

    /*
    for(i = 3; i > -1; i--)
        kprintf("100 / %i = %i\n", i, 100/i);
    */

    void* address = VIRT_ADDR_RAMDISK;
    unsigned int s;
    tar_header_t *t;

    for(i = 0; i<5; i++) {
        t = address;
        nodes[i] = t;
        s = getsize(t->size);
        if(s % 512) s += 512;
        s = ((s>>9)+1)<<9;
        address += s;
    }

    void *elf_start = (void*)nodes[1] + 512;
    void *elf_end = (void*)nodes[2]-1;
    kprintf("\n>>> Attempting to executing program: %s\n", nodes[1]->name);
    kprintf(">>> File is ELF: %p - %p\n", elf_start, elf_end);

    uint32_t entry_point = *(uint32_t*)(elf_start + 24);
    uint32_t ph_offset = *(uint32_t*)(elf_start + 28);
    uint16_t ph_size = *(uint16_t*)(elf_start + 42);
    uint16_t ph_count = *(uint16_t*)(elf_start + 44);

    elf_section_header_t* ph = (elf_start + ph_offset);
    
    kprintf(">>> ENTRY: %p\n", entry_point);
    kprintf(">>> PH: %p, SIZE: %i, COUNT: %i\n", ph_offset, ph_size, ph_count);

    kprintf(">>> off = %p, vaddr = %p, paddr = %p, align = %p\n", ph->p_offset, ph->p_vaddr, ph->p_paddr, ph->align);
    kprintf(">>> filesz = %p, memsz = %p, flags = %p\n", ph->p_filesz, ph->p_memsz, ph->flags);
 
    kprintf("\n[!] Allocating %i bytes of memory at virtual address %p...\n", ph->p_memsz, ph->p_vaddr);

    vmem_map_region(NULL, ph->p_memsz, (void*)ph->p_vaddr, VMEM_FLG_WRITABLE);
    memset((void*)ph->p_vaddr, 0x00, ph->p_memsz);

    kprintf("[!] Copying program data into virtual address space...\n");
    memcpy((void*)ph->p_vaddr, elf_start + ph->p_offset, ph->p_memsz);

    kprintf("[!] Executing Program...\n");
    asm("jmp *%0" : : "r"(entry_point));

    for(;;);
}
