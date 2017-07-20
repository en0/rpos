# rpos

Research Project Operating System.

I have made many attempts and building an operating system and never achieved my goal to satisfaction. After a significant amount of leanring and continuously misplacing my code, i have decided to keep it as simple as possible.

## Goals

- [x] Kernel loads in higher half
- [x] Create flat GDT in kernel .text space
- [x] IDT gate install system where other code can install handlers
- [x] IRQ dispatch system where hardware handlers can install int handers.
- [x] Serial debuging on COM1
- [x] Coredump on FAULT/TRAP
- [x] RTC via PIT Channel 0
- [ ] Setup physical memory manager
- [ ] Setup Virtual memory manager
- [ ] 8MB Stack just under the kernel in the virtual address space.
- [ ] Kernel will pull from it's own small heap at 0xC0400000

## Round 1 Feature Set

- [ ] Kernel will not support multi-tasking but it will so plan on it.
- [ ] Kernel will use an in memory ramdisk loaded as a modual.
- [x] Kernel will use multiboot loader (grub or other existing solution).
- [ ] SYSENTER will be facilitated by NEWLIB but some functions will be stubbed.
- [ ] No real driver system. Keyboard and 0x000b8000 video memory are the only requirements.
- [ ] Kernel will EXEC from boot-time args. The intention is the in-ram-disk path to the shell.
- [ ] Kernel will facilitate EXECs at runtime but will always fall back to the shell specified in the boot arguments.
- [ ] Kernel EXEC will expect a staticly linked ELF executable.

## Known Issues To Fix
- [ ] END flag is not pointing to the correct spot in ram.
 
Example:

(GRUB) kernel (hd0,0)/boot/x86-elf-kernel shell=/bin/sh
* This will execute the shell in user mode. 
* The shell can execute other programs but will not multi-task.
* Shell will be re-executed when the program exits.
    

Required Tools
---

- Cross Compiler with no OS disposition.
- GNU Make or compatable Make system.
- Python
- [QEMU Opensource Processor Emulator](http://qemu.org)

Configuration
---

_config.mk_ contains most of the build directives that you can manipulate.

Building
---

To build the system you must first install or build the required tools and configure your enviroment.

1. Building toolchain.

    The toolchain is, in this case, is GNU GCC and BINUTILS that will not link against GLIBC and targeting i686-elf binaries.
    
    There is a bash file in the scripts directory to build the toolchain for you. Please read through the config.sh file to make sure your folder structure is correct and download the 2 files in download.lst before running the build script. If you run into trouble, take a look at this guid: [GCC_Cross-Compiler](http://wiki.osdev.org/GCC_Cross-Compiler)

2. Configure build.

    Open configure.mk. Verify the compiler names match the ones you just created. Also, feel free to tweek any other settings.
    
    Also, verify that your compiler is in your path. There is a bash script in the scripts directory to add it for you assuming the compiler name is the default.
    
    _$ . scripts/env.sh_
    
3. Review Options.

    You can look at the current env by running _make info_. 
    
    You can also see what targets are available by running _make options_.
    
4. Build the project.

    _$ make_
    
5. Run the project.

    _$ make run-src/kernel_.
    
    You can run other targets the same way. run _make options_ for more details.

---

# Notes: 

I think i am going to start over on some parts - I want to begin with a higher
half kernel (done). Setup physical memory managment then setup paging using a
recursive paging model. After that IRQs then ISRs then probably some elf
handing.  if i can get to the point where i am loading a elf for execution,
even in supervisor mode, i will be happy at this point.

It's been like 3 years since i started this thing, time to make some progress.


## Memory

Virtual Memory Space


```
Start Addr  | End Addr      | Name              | Size
------------|---------------|-------------------|-------
0xFFFFF000  | 0xFFFFFFFF    | PDT               |   4 KB
0xFFC00000  | 0xFFFFEFFF    | PTE [0 - 1023]    |   4 MB
0xFFBE0000  | 0xFFBFFFFF    | Physical Mem Map  | 128 KB
0xFF3E0000  | 0xFFBDFFFF    | Kernel Stack      |   8 MB
0xC0400000  | 0xFF3DFFFF    | Kernel Heap       |  ~1 GB
0xC0000000  | 0xC03FFFFF    | ** KERNEL **      |   4 MB
------------|---------------|-------------------|-------
0xBF800000  | 0xBFFFFFFF    | USER STACK        |   8 MB
0x40000000  | 0xBF7FFFFF    | Shared libs       |   2 GB
----------  | 0x3FFFFFFF    | USER HEAP         |   1 GB
----------  | ----------    | USER .bss         |   -
----------  | ----------    | USER .data        |   -
0x08048000  | ----------    | USER .text        |   -
``` 

## Notes on PIT

CLK frequency generated by the system oscillator is most likey 1.193182 MHz

Control Word format. (I/O port 0x43)

```
[ CS | CS | ACS | ACS | OM | OM | OM | BCD ]

CS : Counter Select
    0 0 : Channel 0 ***
    0 1 : Channel 1
    1 0 : Channel 2

ACS : Access Mode
    0 0 : Latch Count
    0 1 : LSB only
    1 0 : MSB only
    1 1 : LSB and MSB
    
OM : Operating Mode
    0 0 0 : Mode 0
    0 0 1 : Mode 1
    x 1 0 : Mode 2 ***
    x 1 1 : Mode 3
    1 0 0 : Mode 4
    1 0 1 : Mode 5

BCD : Binary Coded Decimal Enabled.
    0 : 16 bit words ***
    1 : for digit BCD
```

Formula to compute time between IRQs where RV is the Counter Value (in Hz)

- frequency = 1193182 / RV
- time in ms = RV / (3579545 / 3) * 1000

Using RV 0x04AA (1194) = 999.315 Hz = ~1 ms. Good enough for RPOS.

### COUNTER 0

This is the only counter connected to IRQs. It will implement the RTC (Mode 2)

- Counter 0 data port: 0x40
- Mode/Command  register: 0x43
- The BIOS ushually sets up Counter 0 to 18.2065 Hz (CR=65535)
- OUT 0 is connected to IRQ 0 via the PIC
- IRQs are generated on the rising edge of OUT 0

RTC implementation 

- Control Word: `0b00110100` (CS=0, ACS=3, OM=2, BCD=0)
- Reload Value: 0x04AA (CRl=0xAA, CRm=0x04)

```asm
; Setup RTC on Counter 0 to produce IRQ 0 at 1 ms intervals.
out $0b00110100, $0x43  ; CS=0, ACS=3, MODE=2, BCD=false
out $0xAA, $0x40        ; LSB of CR
out $0x04, $0x40        ; MSB of CR
```

### Counter 1 

NC and unusable (may not even exist in the system.)

### Counter 2 

connected directly to the PC Speaker

- Counter 2 data port: 0x42
- Mode/Command  register: 0x43
- GATE 2 is connected to IO port 0x61 (bit 0)
- OUT 2 is connected to IO port 0x61 (bit 5)
