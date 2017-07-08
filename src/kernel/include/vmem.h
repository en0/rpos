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

#ifndef __VMEM_H
#define __VMEM_H 1

#include <config.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef ARCH_x86
#include <x86_vmem.h>

/* Types */
#define vmem_context            x86_vmem_context
#define phys_addr               x86_phys_addr
#define virt_addr               x86_virt_addr

/* FLAGS */
#define VMEM_FLG_PRESENT        x86_FLG_VMEM_PRESENT
#define VMEM_FLG_WRITABLE       x86_FLG_VMEM_WRITABLE
#define VMEM_FLG_SUPERVISOR     x86_FLG_VMEM_SUPERVISOR
#define VMEM_FLG_WRITETHROUGH   x86_FLG_VMEM_WRITETHROUGH
#define VMEM_FLG_NOT_CACHEABLE  x86_FLG_VMEM_NOT_CACHEABLE
#define VMEM_FLG_ACCESSED       x86_FLG_VMEM_ACCESSED
#define VMEM_FLG_RESEVED        x86_FLG_VMEM_RESEVED
#define VMEM_FLG_SIZE           x86_FLG_VMEM_SIZE
#define VMEM_FLG_GLOBAL         x86_FLG_VMEM_GLOBAL
#define VMEM_FLG_COPY           x86_FLG_VMEM_COPY

/* METHODS */
#define vmem_init               x86_vmem_init
#define vmem_copy_context       x86_vmem_copy_context
#define vmem_destroy_context    x86_vmem_destroy_context
#define vmem_kalloc             x86_vmem_kalloc
#define vmem_kfree              x86_vmem_kfree
#define vmem_map_region         x86_vmem_map_region
#define vmem_activate           x86_vmem_activate

#endif /** ARCH_x86 **/

#endif /* __VMEM_H */

