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

#define phys_addr           x86_phys_addr
#define virt_addr           x86_virt_addr

#define VMEM_PRESENT        x86_VMEM_PRESENT
#define VMEM_WRITABLE       x86_VMEM_WRITABLE
#define VMEM_USER           x86_VMEM_USER
#define VMEM_WRITETHROUGH   x86_VMEM_WRITETHROUGH
#define VMEM_NOT_CACHEABLE  x86_VMEM_NOT_CACHEABLE
#define VMEM_ACCESSED       x86_VMEM_ACCESSED
#define VMEM_DIRT           x86_VMEM_DIRT
#define VMEM_PAT            x86_VMEM_PAT
#define VMEM_CPU_GLOBAL     x86_VMEM_CPU_GLOBAL
#define VMEM_LV4_GLOBAL     x86_VMEM_LV4_GLOBAL

#define vmem_init           x86_vmem_init
#define vmem_mmap           x86_vmem_mmap
#define vmem_enable         x86_vmem_enable

#endif /** ARCH_x86 **/

#endif /* __VMEM_H */

