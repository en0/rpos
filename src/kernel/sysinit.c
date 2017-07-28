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
#include <boot/multiboot.h>
#include <cpu.h>

#include <irq/idt.h>
#include <irq/irq.h>

#include <mem/gdt.h>
#include <mem/pmem.h>
#include <mem/vmem.h>
#include <mem/heap.h>

#include <cpu/fault.h>
#include <cpu/rtc.h>

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

void declare_physical_memory(multiboot_info_t* mbi) {

    multiboot_memory_map_t *mmap;

    // Free the memory that the bootloader identified as available.
    for(mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
        (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
        mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {

        if(mmap->type == 1) {
            pmem_free_region((void*)((uint32_t)mmap->addr), (uint32_t)mmap->len);
        }
    }
}

void lock_used_memory(multiboot_info_t* mbi) {

    // Lock kernel memory, the memory map, and the stack region
    pmem_lock_region(PHYS_ADDR_KSTART, KERNEL_SIZE);

    // Lock the first page. it just messes up NULL checking
    pmem_lock_region(0x00, 1);

    // Lock video memory
    pmem_lock_region(PHYS_ADDR_VGA3, 1);

    // Lock the modules info structure and ramdisk module.
    multiboot_module_t *ramdisk_info = (multiboot_module_t *)mbi->mods_addr;
    pmem_lock_region((void*)mbi->mods_addr, sizeof(uint32_t));
    pmem_lock_region((void*)ramdisk_info->mod_start, ramdisk_info->mod_end - ramdisk_info->mod_start);
}

void map_used_memory(multiboot_info_t* mbi) {

    // Map the kernel memory
    vmem_map_region(
        PHYS_ADDR_KSTART,
        KERNEL_SIZE,
        VIRT_ADDR_KSTART,
        VMEM_FLG_WRITABLE
    );

    // Map the ramdisk
    multiboot_module_t *ramdisk_info = (multiboot_module_t *)mbi->mods_addr;
    vmem_map_region(
        (void*)ramdisk_info->mod_start,
        ramdisk_info->mod_end - ramdisk_info->mod_start,
        VIRT_ADDR_RAMDISK,
        0x00 // Read only
    );

    // Map video memory
    vmem_map_region(
        PHYS_ADDR_VGA3,
        4096,
        VIRT_ADDR_VGA3,
        VMEM_FLG_WRITABLE
    );

    // Map 8MB Stack Space
    vmem_map_region(
        NULL,  // Get new page frames.
        (uint32_t)(VIRT_ADDR_STACK - VIRT_ADDR_ESTACK),
        VIRT_ADDR_ESTACK, 
        VMEM_FLG_WRITABLE
    );
}

void initialize_memory_management(multiboot_info_t *mbi) {

    /*
     * Setup a memory management system that will allow the kernel to allocate
     * memory safely.
     */

    // Install a flat GDT.
    gdt_setup();

    // Setup physical frame allocation manager and lock used memory spaces.
    pmem_setup();
    declare_physical_memory(mbi);
    lock_used_memory(mbi);

    // Setup virtual memory manager and map used memory spaces.
    vmem_setup();
    map_used_memory(mbi);

    // Enable the new memory map.
    vmem_enable();

    // Setup the heap manager
    heap_create(VIRT_ADDR_HEAP, VIRT_ADDR_EHEAP, VMEM_FLG_WRITABLE);
}

void initialize_interrupt_core(multiboot_info_t* mbi) {
    idt_setup();
    irq_setup();
}

void initialize_peripherals() {
    rtc_setup();
}

void system_init(multiboot_info_t* mbi) {
#ifdef PROFILE_DEBUG
    dbg_setup();
#endif
    validate_boot_env(mbi);
    initialize_memory_management(mbi);
    initialize_interrupt_core(mbi);
    initialize_peripherals();
    fault_setup();
}

