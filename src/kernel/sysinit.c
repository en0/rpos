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

#include <config.h>
#include <kernel.h>
#include <multiboot.h>
#include <cpu.h>
#include <pmem.h>
#include <vmem.h>
#include <kprint.h>

extern void initGDT();
extern void initIDT();
extern void initIRQ();
extern void initFAULT();
extern void initRTC();

#ifdef PROFILE_DEBUG
extern void initDBG();
#endif

/**
 ** CHECK_FLAG is used to validate flags from 
 ** multiboot. 
 **/

#define CHECK_FLAG(a,b) ((a & b) == b)


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

    initPMEM();

    multiboot_memory_map_t *mmap;

    // Free the memory that the bootloader says is physicly available.
    for(mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
        (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
        mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {

        if(mmap->type == 1) {
            pmem_free_region((void*)((uint32_t)mmap->addr), (uint32_t)mmap->len);
        }
    }

    // Lock kernel memory, the memory map, and the stack region
    pmem_lock_region(PHYS_ADDR_KSTART, KERNEL_SIZE);

    // Lock the first page. it just messes up NULL checking
    pmem_lock_region(0x00, 1);
}

void init_vmem(multiboot_info_t* mbi) {

    initVMEM();

    vmem_map_region(
        PHYS_ADDR_KSTART,
        KERNEL_SIZE,
        VIRT_ADDR_KSTART,
        VMEM_FLG_WRITABLE
    );

    vmem_enable();
}

void init_video() {

    initKPRINT(VIRT_ADDR_VGA3);

    pmem_lock_region(PHYS_ADDR_VGA3, 1);

    vmem_map_region(
        PHYS_ADDR_VGA3,
        4096,
        VIRT_ADDR_VGA3,
        VMEM_FLG_WRITABLE
    );
}

void init_stack() {
    // You cannot install the new stack from here but you can
    // allocate the space and map the addresses
}

void system_init(multiboot_info_t* mbi) {

    validate_boot_env(mbi);

#ifdef PROFILE_DEBUG
    initDBG();
#endif

    init_pmem(mbi);
    init_vmem(mbi);
    init_stack();
    init_video();

    initGDT();
    initIDT();
    initIRQ();
    initFAULT();

    initRTC();

    sti();

}

