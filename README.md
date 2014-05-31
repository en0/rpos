rpos
====

Research Project Operating System.

Just playing around with systems programming.

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
    
3. Review Options.

    You can look at the current env by running _make info_. 
    
    You can also see what targets are available by running _make options_.
    
4. Build the project.

    _make_
    
5. Run the project.

    _make run-src/kernel_.
    
    You can run other targets the same way. run _make options_ for more details.
