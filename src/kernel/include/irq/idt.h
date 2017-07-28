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

#ifndef __IDT_H
#define __IDT 1

#include <stdint.h>

#define IDT_FLG_PRESENT  0x8000
#define IDT_FLG_DPL0     0x0000
#define IDT_FLG_DPL1     0x2000
#define IDT_FLG_DPL2     0x4000
#define IDT_FLG_DPL3     0x6000
#define IDT_FLG_TASKGATE 0x0500
#define IDT_FLG_INTEGATE 0x0E00
#define IDT_FLG_TRAPGATE 0x0F00

void idt_setup();
void idt_setGate(uint32_t, void(*)(), uint32_t);

#endif /* __IDT_H */
