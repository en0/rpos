# rpos

Research Project Operating System.

I have made many attempts and building an operating system and never achieved
my goal to satisfaction. After a significant amount of leanring and
continuously misplacing my code, I have decided to keep it as simple as
possible.

[Link to docs](docs/README.md)

## Goals

- [x] Kernel loads in higher half
- [x] Create flat GDT in kernel .text space
- [x] IDT gate install system where other code can install handlers
- [x] IRQ dispatch system where hardware handlers can install int handers.
- [x] Serial debuging on COM1
- [x] Coredump on FAULT/TRAP
- [x] RTC via PIT Channel 0
- [x] Setup physical memory manager
- [x] Setup Virtual memory manager
- [x] 8MB Stack just under the kernel in the virtual address space.
- [ ] Kernel will pull from it's own small heap at 0xC0400000

## Round 1 Feature Set

- [ ] Kernel will not support multi-tasking but it will so plan on it.
- [x] Kernel will use an in memory ramdisk loaded as a modual.
- [x] Kernel will use multiboot loader (grub or other existing solution).
- [ ] SYSENTER will be facilitated by NEWLIB but some functions will be stubbed.
- [ ] No real driver system. Keyboard and 0x000b8000 video memory are the only requirements.
- [ ] Kernel will EXEC from boot-time args. The intention is the in-ram-disk path to the shell.
- [ ] Kernel will facilitate EXECs at runtime but will always fall back to the shell specified in the boot arguments.
- [ ] Kernel EXEC will expect a staticly linked ELF executable.

## Known Issues To Fix
- [x] END flag is not pointing to the correct spot in ram.
 
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
