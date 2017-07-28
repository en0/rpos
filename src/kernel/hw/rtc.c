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

#include <cpu.h>
#include <irq.h>
#include <kprint.h>
#include <debug.h>

uint32_t _seconds;
uint16_t _ticks;

void rtc_IRQHandler();

void initRTC() {

    /* 
     * Configure the PIT
     *   Channel 0, Rate Interval, L/MSB, 16bit Word
     *   Count Register=0x04AA which is about 1 ms per tick.
     */

    outb(0x34, 0x43); // Control Word: 0b00110100 (CS=0,ACS=3,MODE=2,BCD=false)
    outb(0xAA, 0x40); // Reload Value: 0x04AA ~= 1 ms
    outb(0x04, 0x40);

    // Initialize the tick tracking
    _ticks = _seconds = 0;

    // Install IRQ Hander
    install_irq_handler(0, &rtc_IRQHandler);
}

uint32_t rtc_getTicks() {
    return _seconds * 1000 + _ticks;
}

void rtc_IRQHandler(regs_t *r) { 
    if(++_ticks == 1000) {
        _ticks = 0;
        _seconds++;
#ifdef PROFILE_DEBUG
        dbg_printf("rtc_getTicks() = %i\n", rtc_getTicks());
#endif
    }
}
