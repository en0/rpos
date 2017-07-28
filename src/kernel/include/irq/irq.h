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

#ifndef __IIRQ_H
#define __IIRQ_H 1

#include <cpu/core.h>

#define PIC0            0x20
#define PIC0_DATA       0x21
#define PIC1            0xA0
#define PIC1_DATA       0xA1

#define ICW1_ICW4       0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE     0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4  0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL      0x08		/* Level triggered (edge) mode */
#define ICW1_INIT       0x10		/* Initialization - required! */
 
#define ICW4_8086       0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO       0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE  0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C		/* Buffered mode/master */
#define ICW4_SFNM       0x10		/* Special fully nested (not) */

#define PIC_EOI         0x20

void irq_setup(void);
void dispatch_irq_handler(regs_t *r);

void irq_stub0();
void irq_stub1();
void irq_stub2();
void irq_stub3();
void irq_stub4();
void irq_stub5();
void irq_stub6();
void irq_stub7();
void irq_stub8();
void irq_stub9();
void irq_stub10();
void irq_stub11();
void irq_stub12();
void irq_stub13();
void irq_stub14();
void irq_stub15();

#endif /** __IIRQ_H **/
