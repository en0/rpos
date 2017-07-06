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

#ifndef __X86_VMEM_H
#define __X86_VMEM_H 1

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Flags */
#define x86_FLG_VMEM_PRESENT        0b00000000001
#define x86_FLG_VMEM_WRITABLE       0b00000000010
#define x86_FLG_VMEM_SUPERVISOR     0b00000000100
#define x86_FLG_VMEM_WRITETHROUGH   0b00000001000
#define x86_FLG_VMEM_NOT_CACHEABLE  0b00000010000
#define x86_FLG_VMEM_ACCESSED       0b00000100000
#define x86_FLG_VMEM_RESEVED        0b00001000000
#define x86_FLG_VMEM_SIZE           0b00010000000
#define x86_FLG_VMEM_GLOBAL         0b00100000000
#define x86_FLG_VMEM_COPY           0b01000000000

/* Types */
typedef uint32_t x86_vmem_context;
typedef uint32_t x86_virt_addr;
typedef uint32_t x86_phys_addr;

/* Methods */
x86_vmem_context* x86_vmem_init(x86_virt_addr*, x86_phys_addr*, size_t);
x86_vmem_context* x86_vmem_copy_context(x86_vmem_context*);
void x86_vmem_destroy_context(x86_vmem_context*);
x86_virt_addr* x86_vmem_kalloc(x86_vmem_context*, x86_virt_addr*, size_t, uint32_t);
void x86_vmem_kfree(x86_vmem_context*, x86_virt_addr*, size_t);
void x86_vmem_map_region(x86_vmem_context*, x86_virt_addr*, x86_phys_addr*, size_t);

#endif /* __X86_VMEM_H */

