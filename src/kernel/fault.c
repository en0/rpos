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

/** 
 * Some of this code was addapted from 
 ** http://www.osdever.net/tutorials/view/brans-kernel-development-tutorial
 **/


#include <cpu.h>
#include <string.h>
#include <kprint.h>
#include <debug.h>

const char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoin",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present.",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Security Exception",
    "Reserved",
    "Triple Fault",
    "FPU Error Interrupt"
};

extern void fault_stub0(void);
extern void fault_stub1(void);
extern void fault_stub2(void);
extern void fault_stub3(void);
extern void fault_stub4(void);
extern void fault_stub5(void);
extern void fault_stub6(void);
extern void fault_stub7(void);
extern void fault_stub8(void);
extern void fault_stub10(void);
extern void fault_stub11(void);
extern void fault_stub12(void);
extern void fault_stub13(void);
extern void fault_stub14(void);
extern void fault_stub16(void);
extern void fault_stub17(void);
extern void fault_stub18(void);
extern void fault_stub19(void);
extern void fault_stub20(void);
extern void fault_stub30(void);

void initFAULT() {
    idt_setGate(0x00, &fault_stub0, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x01, &fault_stub1, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x02, &fault_stub2, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x03, &fault_stub3, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x04, &fault_stub4, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x05, &fault_stub5, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x06, &fault_stub6, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x07, &fault_stub7, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x08, &fault_stub8, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    // IRQ 9 is obsolete.
    idt_setGate(0x0A, &fault_stub10, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x0B, &fault_stub11, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x0C, &fault_stub12, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x0D, &fault_stub13, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x0E, &fault_stub14, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    // IRQ 15 is reserved by intel
    idt_setGate(0x10, &fault_stub16, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x11, &fault_stub17, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x12, &fault_stub18, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x13, &fault_stub19, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    idt_setGate(0x14, &fault_stub20, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
    // IRQ 21 - 29 are reserved by intel.
    idt_setGate(0x1E, &fault_stub30, IDT_FLG_PRESENT | IDT_FLG_DPL0 | IDT_FLG_INTEGATE);
}

void fault_handler(regs_t *r) {

#ifdef PROFILE_DEBUG
    dbg_printf("\n\n!!! FAULT %i(%i) - %s !!!\n\n", r->int_no, r->err_code, exception_messages[r->int_no]);
    dbg_printf("--- CORE DUMP -----------------------------------------------------\n");
    dbg_printf("eax: %p, ebx: %p, ecx: %p, edx: %p\n", r->eax, r->ebx, r->ecx, r->edx);
    dbg_printf("esp: %p, ebp: %p, esi: %p, edi: %p\n", r->esp, r->ebp, r->esi, r->edi);
    dbg_printf("cs: %p, ss: %p, ds: %p, es: %p\n", r->cs, r->ss, r->ds, r->es);
    dbg_printf("fs: %p, gs: %p, eip: %p, eflags: %b\n", r->fs, r->gs, r->eip, r->eflags);
#endif

    kclear();
    kprintf("\n\n!!! FAULT %i(%i) - %s !!!\n\n", r->int_no, r->err_code, exception_messages[r->int_no]);
    kprintf("--- CORE DUMP -----------------------------------------------------\n");
    kprintf("eax: %p, ebx: %p, ecx: %p, edx: %p\n", r->eax, r->ebx, r->ecx, r->edx);
    kprintf("esp: %p, ebp: %p, esi: %p, edi: %p\n", r->esp, r->ebp, r->esi, r->edi);
    kprintf("cs: %p, ss: %p, ds: %p, es: %p\n", r->cs, r->ss, r->ds, r->es);
    kprintf("fs: %p, gs: %p, eip: %p, eflags: %b\n", r->fs, r->gs, r->eip, r->eflags);

    // The stubs do an iRet because i am sure i will have
    // some fault hanlders return back. I might even do a vector map
    // so that things like the PMEM code can assert there own fault_handler
    // for specific interrupts.

    halt();
}
