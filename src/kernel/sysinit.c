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

#include <multiboot.h>
#include <pmem.h>
#include <vmem.h>

/** 
 ** _start and _end are exposed via linker script 
 ** to identify the begining and end of the kernel
 ** image. The values in these pointers are not 
 ** valid. It is the location of the symbol that is
 ** important.
 ** 
 ** The 2 macros, _START and _END, are defined to 
 ** ensure the proper use of these values.
 **/

extern void* _start;
extern void* _end;

#define _START ((void*)&_start)
#define _END ((void*)&_end)


/**
 ** CHECK_FLAG is used to validate flags from 
 ** multiboot. 
 **/

#define CHECK_FLAG(a,b) ((a & b) == b)


/**
 ** kabort is defined in startup.s. It will
 ** print the provided error message and halt the 
 ** system.
 **/

extern void kabort(const char*);

void* find_stack(multiboot_info_t* mbi) {
    return pmem_map_end(_END, (mbi->mem_upper + mbi->mem_lower)) + STACK_SIZE;
}

void validate_boot_env(multiboot_info_t* mbi) {

    /** 
     ** Required Information
     ** 1. lower/upper memory info
     ** 2. Full Memory Map
     ** 3. Either modules OR boot device with drive info
     **
     ** Optional Items 
     ** 1. Command Line
     ** 2. Boot Device **
     **    a. Drive Info
     ** 3. Modules **
     **
     ** Ignore Items
     ** 1. Symbol Table
     ** 2. Config Table
     ** 3. Boot Loader Name
     ** 4. APM Table ??
     ** 5. Video Info ??
     **/

    /** MBOOT Sanity Check **/
    if( CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_AOUT_SYMS) && 
        CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_ELF_SHDR)) {
        kabort("ERROR: Multiboot data is not sane!\n");
    }

    /** Need memory info **/
    if( !CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_MEM_MAP) ||
        !CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_MEMORY)) {
        kabort("ERROR: No memory information provided by boot loader!\n");
    }

    /** Need a boot device **/
    if( !CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_MODS) &&
      ( !CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_BOOTDEV) ||
        !CHECK_FLAG(mbi->flags, MULTIBOOT_INFO_DRIVE_INFO))) {
        kabort("ERROR: No boot device!\n");
    }
}

void init_pmem(multiboot_info_t* mbi) {

    void* STACK_END = find_stack(mbi);

    pmem_init(_END, (mbi->mem_upper + mbi->mem_lower));

    multiboot_memory_map_t *mmap;
    for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
        (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
        mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {

        if(mmap->type == 1) {
            pmem_free_region((void*)((uint32_t)mmap->addr), (uint32_t)mmap->len);
        }
    }

    // Lock kernel memory, the memory map, and the stack region
    pmem_lock_region(_START, (STACK_END - _START));

    // Lock the first page. it just messes up NULL checking
    pmem_lock_region(0x00, 4096);
}

void init_vmem() {

}

void system_init(multiboot_info_t* mbi) {
    validate_boot_env(mbi);
    init_pmem(mbi);
    init_vmem();
}

