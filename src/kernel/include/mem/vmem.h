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
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef __VMEM_H
#define __VMEM_H 1

/* Flags */
#define VMEM_FLG_PRESENT        0b00000000001
#define VMEM_FLG_WRITABLE       0b00000000010
#define VMEM_FLG_USER           0b00000000100
#define VMEM_FLG_WRITETHROUGH   0b00000001000
#define VMEM_FLG_NOCACHE        0b00000010000
#define VMEM_FLG_ACCESSED       0b00000100000
#define VMEM_FLG_DIRTY          0b00001000000
#define VMEM_FLG_LARGE          0b00010000000
#define VMEM_FLG_GLOBAL         0b00100000000

typedef uint32_t phys_addr;
typedef uint32_t virt_addr;

void initVMEM();
void vmem_map_address(phys_addr*, virt_addr*, uint16_t);
void vmem_map_region(phys_addr*, size_t, virt_addr*, uint16_t);
void vmem_enable();

#endif /* __VMEM_H */
