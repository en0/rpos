                .global _start

                ## Multiboot Flags
                .set ALIGN, 1<<0
                .set MEMINFO, 1<<1

                ## Compute Multiboot header values
                .set FLAGS, ALIGN | MEMINFO
                .set MAGIC, 0x1BADB002
                .set CHECKSUM, -(MAGIC + FLAGS)


                .section .multiboot

                ## Multiboot Header

                .align 4
                .long MAGIC
                .long FLAGS
                .long CHECKSUM


                .section .text

                ## Kernel Entry 

_start:         jmp _start
