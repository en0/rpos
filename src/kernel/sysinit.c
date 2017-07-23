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

    // Lock the modules info structure and ramdisk module.
    multiboot_module_t *ramdisk_info = (multiboot_module_t *)mbi->mods_addr;
    pmem_lock_region(
        (void*)mbi->mods_addr,
        sizeof(uint32_t));
    pmem_lock_region(
        (void*)ramdisk_info->mod_start,
        ramdisk_info->mod_end - ramdisk_info->mod_start );
}

void init_vmem(multiboot_info_t* mbi) {

    initVMEM();

    // Map kernel memory.
    vmem_map_region(
        PHYS_ADDR_KSTART,
        KERNEL_SIZE,
        VIRT_ADDR_KSTART,
        VMEM_FLG_WRITABLE
    );

    // Map the ramdisk into the virtual address space.
    multiboot_module_t *ramdisk_info = (multiboot_module_t *)mbi->mods_addr;
    vmem_map_region(
        (void*)ramdisk_info->mod_start,
        ramdisk_info->mod_end - ramdisk_info->mod_start,
        VIRT_ADDR_RAMDISK,
        0x00 // Read only
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

uint32_t init_stack() {

    /* This should be called from start.s although it is not required. What is
     * required is that we change the stack pointer in the top frame (.start)
     * to avoid needing to do a complicated relocation. */

    uint32_t len = VIRT_ADDR_STACK - VIRT_ADDR_ESTACK;
    vmem_map_region(NULL, len, VIRT_ADDR_ESTACK, VMEM_FLG_WRITABLE | VMEM_FLG_GLOBAL);

    return (uint32_t)VIRT_ADDR_STACK;
}

void system_init(multiboot_info_t* mbi) {

    validate_boot_env(mbi);

#ifdef PROFILE_DEBUG
    initDBG();
#endif

    init_pmem(mbi);
    init_vmem(mbi);
    init_video();

    initGDT();
    initIDT();
    initIRQ();
    initFAULT();

    initRTC();
}

