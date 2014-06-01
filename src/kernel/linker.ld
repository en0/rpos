ENTRY(_start)
SECTIONS
{
    /* Begin putting sections at 1 MiB, a conventional place for kernels to be loaded at by the bootloader. */
    . = 1M;

    /* First put the multiboot header, as it is required to be put very early
     * early in the image or the bootloader won't recognize the file format.
     * Next we'll put the .text section. */
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    /* Read-only data. */
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    /* Read-write data (initialized) */
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    /* Read-write data (uninitialized) and stack */
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)

        _KERNEL_END_ = .;

        /* This stack refrence is temporary. We will exclude it from the locked memory because
         * we will be setting up a new stack in the pmemory manager and we want to reclaime ths
         * space. That is why kernel end does not take the tstack region into account. */
        *(.tstack)
    }
}
