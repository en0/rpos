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

#include <string.h>
#include <kernel.h>
#include <cpu.h>
#include <irq/idt.h>
#include <irq/irq.h>

#define woutb(a,b) outb(a,b); io_wait()

void remap_pic(uint32_t, uint32_t);
void disable_interrupt(uint32_t);
void enable_interrupt(uint32_t);

void(*_vectors[16])(regs_t*);

void irq_setup() {

    // Remap PIC0 to 0x20-0x27 and PIC1 to 0x28-0x2F
    remap_pic(0x20, 0x28);

    // Dont think i need to do this.
    memset((void*)_vectors, 0x00, sizeof(int));

    // Mask off all IRQs
    disable_interrupt(0);
    disable_interrupt(1);
    disable_interrupt(2);
    disable_interrupt(3);
    disable_interrupt(4);
    disable_interrupt(5);
    disable_interrupt(6);
    disable_interrupt(7);
    disable_interrupt(8);
    disable_interrupt(9);
    disable_interrupt(10);
    disable_interrupt(11);
    disable_interrupt(12);
    disable_interrupt(13);
    disable_interrupt(14);
    disable_interrupt(15);

    // Install all idt gates
    idt_setGate(0x20, &irq_stub0, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x21, &irq_stub1, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x22, &irq_stub2, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x23, &irq_stub3, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x24, &irq_stub4, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x25, &irq_stub5, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x26, &irq_stub6, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x27, &irq_stub7, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x28, &irq_stub8, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x29, &irq_stub9, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x2a, &irq_stub10, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x2b, &irq_stub11, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x2c, &irq_stub12, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x2d, &irq_stub13, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x2e, &irq_stub14, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x2f, &irq_stub15, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
}

void disable_interrupt(uint32_t n) {
    uint16_t port;
    uint8_t val;

    if(n < 8) {
        port = PIC0_DATA;
    } else {
        port = PIC1_DATA;
        n -= 8;
    }

    val = inb(port) | (1<<n);
    outb(val, port);
}

void enable_interrupt(uint32_t n) {
    uint16_t port;
    uint8_t val;

    if(n < 8) {
        port = PIC0_DATA;
    } else {
        port = PIC1_DATA;
        n -= 8;
    }

    val = inb(port) & ~(1<<n);
    outb(val, port);
}

void remap_pic(uint32_t offset_pic0, uint32_t offset_pic1) {

    uint8_t mask_pic0, mask_pic1;

    mask_pic0 = inb(PIC0);
    mask_pic1 = inb(PIC1);

    woutb(ICW1_INIT | ICW1_ICW4, PIC0); // Initialize pic0 with cascade mode
    woutb(ICW1_INIT | ICW1_ICW4, PIC1); // Initialize pic1 with cascade mode

	woutb(offset_pic0, PIC0_DATA);      // ICW2: Master PIC vector offset
	woutb(offset_pic1, PIC1_DATA);      // ICW2: Slave PIC vector offset
	woutb(4, PIC0_DATA);                // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	woutb(2, PIC1_DATA);                // ICW3: tell Slave PIC its cascade identity (0000 0010)
 
	woutb(ICW4_8086, PIC0_DATA);
	woutb(ICW4_8086, PIC1_DATA);
 
	outb(mask_pic0, PIC0_DATA);         // restore saved masks.
	outb(mask_pic1, PIC1_DATA);

}

void dispatch_irq_handler(regs_t *r) {
    if(_vectors[r->int_no] != NULL)
        _vectors[r->int_no](r);
#ifdef PROFILE_DEBUG
    else 
        dbg_printf("Unhandled IRQ: %i\n", r->int_no);
#endif

    if(r->int_no >= 8) outb(PIC_EOI, PIC1);
    outb(PIC_EOI, PIC0);
}

void install_irq_handler(int irq, void(*fn)(regs_t*)) {

    // Install the fn route in the vector that irqEntryN will call
    _vectors[irq] = fn;

    // Enable the IRQ in on the PIC
    enable_interrupt(irq);
}

