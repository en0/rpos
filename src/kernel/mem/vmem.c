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

#include <stdbool.h>
#include <kernel.h>
#include <string.h>
#include <debug.h>
#include <memory.h>

uint32_t* _vmap_dir;
uint32_t* _vmap_tbl = VIRT_ADDR_PGPTE;
bool _vmap_enabled;

#define FLAG(v,f) ((v&f)==f)

void printPDE(virt_addr *v) {
    
#ifdef PROFILE_DEBUG

    uint32_t dir_index = (uint32_t)v >> 22;
    uint32_t tbl_index = _vmap_enabled ? (uint32_t)v >> 12 : ((uint32_t)v >> 12) & 0x3FF;
    uint32_t* tbl = _vmap_enabled ? _vmap_tbl : (uint32_t*)(_vmap_dir[dir_index] & 0xFFFFF000);

    dbg_printf("(%p)->vmap_dir[%i] = %p\n", v, dir_index, _vmap_dir[dir_index]);
    if((_vmap_dir[dir_index] & VMEM_FLG_PRESENT) == VMEM_FLG_PRESENT) {
        dbg_printf(" PDE> PT Physical Address: %p\n", _vmap_dir[dir_index] & 0xFFFFF000);
        dbg_printf(" PDE> FLAGS: %s%s%s%s%s%s%s%s (%b)\n", 
            FLAG(_vmap_dir[dir_index], VMEM_FLG_PRESENT) ? "PRESENT " : "",
            FLAG(_vmap_dir[dir_index], VMEM_FLG_WRITABLE) ? "WRITEABLE " : "",
            FLAG(_vmap_dir[dir_index], VMEM_FLG_USER) ? "USER " : "",
            FLAG(_vmap_dir[dir_index], VMEM_FLG_WRITETHROUGH) ? "WRITETHROUGH " : "",
            FLAG(_vmap_dir[dir_index], VMEM_FLG_NOCACHE) ? "NOCACHE " : "",
            FLAG(_vmap_dir[dir_index], VMEM_FLG_ACCESSED) ? "ACCESSED " : "",
            FLAG(_vmap_dir[dir_index], VMEM_FLG_LARGE) ? "LARGE " : "",
            FLAG(_vmap_dir[dir_index], VMEM_FLG_GLOBAL) ? "GLOBAL " : "",
            _vmap_dir[dir_index] & 0x00000FFF
        );

        dbg_printf(" PTE> PG Physical Address: %p\n", tbl[tbl_index] & 0xFFFFF000);
        dbg_printf(" PTE> FLAGS: %s%s%s%s%s%s%s%s (%b)\n", 
            FLAG(tbl[tbl_index], VMEM_FLG_PRESENT) ? "PRESENT " : "",
            FLAG(tbl[tbl_index], VMEM_FLG_WRITABLE) ? "WRITEABLE " : "",
            FLAG(tbl[tbl_index], VMEM_FLG_USER) ? "USER " : "",
            FLAG(tbl[tbl_index], VMEM_FLG_WRITETHROUGH) ? "WRITETHROUGH " : "",
            FLAG(tbl[tbl_index], VMEM_FLG_NOCACHE) ? "NOCACHE " : "",
            FLAG(tbl[tbl_index], VMEM_FLG_ACCESSED) ? "ACCESSED " : "",
            FLAG(tbl[tbl_index], VMEM_FLG_DIRTY) ? "DIRTY " : "",
            FLAG(tbl[tbl_index], VMEM_FLG_GLOBAL) ? "GLOBAL " : "",
            tbl[tbl_index] & 0x00000FFF
        );
        /*
        */
    } else {
        dbg_printf(" PDE> PT NOT Present\n");
    }

    dbg_printf("\n");

#endif

}

void initVMEM() {

    // VMAP is not yet in use.
    _vmap_enabled = false;

    // Get a page frame to hold the page directory
    _vmap_dir = pmem_alloc();

    // all page directory entries are empty
    memset(_vmap_dir, 0x00, 1024*sizeof(uint32_t));

    // Map the directory to itself so that we can refrence the page directory
    // at virtual address 0xFFFFF000 (Recursive map).
    _vmap_dir[(uint32_t)VIRT_ADDR_PGPDE >> 22] = (uint32_t)_vmap_dir | VMEM_FLG_PRESENT | VMEM_FLG_WRITABLE | VMEM_FLG_GLOBAL;

    // Fill all page directory entries with a valid reference to a page table.
    uint32_t i, *tbl;
    for(i = 0; i < ((uint32_t)VIRT_ADDR_PGPDE >> 22); i++) {
        tbl = pmem_alloc();
        _vmap_dir[i] = (uint32_t)tbl | VMEM_FLG_PRESENT | VMEM_FLG_WRITABLE;
        memset(tbl, 0x00, 0x1000);
    }
}

void _vmem_map_address_ident(phys_addr *p, virt_addr *v, uint16_t flags) {
    uint32_t* tbl = (void*)(_vmap_dir[(uint32_t)v >> 22] & 0xFFFFF000);
    tbl[((uint32_t)v>>12) & 0x3ff] = ((uint32_t)p & 0xFFFFF000) | flags | VMEM_FLG_PRESENT;
}

void vmem_map_address(phys_addr *p, virt_addr *v, uint16_t flags) {

    if(p == NULL)
        p = pmem_alloc();

#ifdef PROFILE_DEBUG
    dbg_printf("Mapping %p => %p\n", v, p);
#endif

    if(!_vmap_enabled)
        return _vmem_map_address_ident(p, v, flags);
    else
        _vmap_tbl[(uint32_t)v>>12] = ((uint32_t)p & 0xFFFFF000) | flags | VMEM_FLG_PRESENT;
}

void vmem_map_region(phys_addr *p, size_t len, virt_addr *v, uint16_t flags) {
    uint32_t pa, va;
    for(pa = (uint32_t)p & 0xFFFFF000,
        va = (uint32_t)v & 0xFFFFF000; 
        pa < ((uint32_t)p+len); 
        pa+=0x1000, va+=0x1000)
        if(p==NULL) vmem_map_address(NULL, (void*)va, flags);
        else vmem_map_address((void*)pa, (void*)va, flags);
}

void vmem_enable() {
    asm volatile("mov %0, %%cr3;" : : "r"(_vmap_dir));
    _vmap_enabled = true;
    _vmap_dir = VIRT_ADDR_PGPDE;
}
