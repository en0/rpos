[<< Back](index.md)

# Memory Management Planning

I want this kernel to live in the Higher Half (3G range) of virtual memory. To
do this, I used large 4MB pages to identity map the lower half of memory and
then maped the first 4M of memory (where our kernel is loaded) to the 3G range
(where we want to pretend our kernel is loaded).

## On Boot:

1. Identity map the first 3G of memory.
2. Map address 0xc0000000 to the first 4MB of memory

## Sysinit

1. Initialize physical memory manager
2. Lock kernel address space.
3. Lock module load locations (from boot loader)
4. Initialize virtual memory manager
5. Remap the higher half to a new table structure using 4KB frames.
6. Allocate, Map, and Relocate STACK to appropriate virtual address.

<sup>Note that the identity map is not rebuilt when the virtual memory is
remapped.<sup>

# Virtual Address Space

```
Start Addr  | End Addr      | Name              | Size
------------|---------------|-------------------|-------
0xFFFFF000  | 0xFFFFFFFF    | PDT               |   4 KB
0xFFC00000  | 0xFFFFEFFF    | PTE [0 - 1023]    |   4 MB
0xFF400000  | 0xFFBFFFFF    | Kernel Stack      |   8 MB
0xC0400000  | 0xFF3FFFFF    | Kernel Heap       |  ~1 GB
0xC0000000  | 0xC03FFFFF    | ** KERNEL **      |   4 MB
------------|---------------|-------------------|-------
0xBF800000  | 0xBFFFFFFF    | USER STACK        |   8 MB
0x40000000  | 0xBF7FFFFF    | Shared libs       |   2 GB
----------  | 0x3FFFFFFF    | USER HEAP         |   1 GB
----------  | ----------    | USER .bss         |   -
----------  | ----------    | USER .data        |   -
0x08048000  | ----------    | USER .text        |   -
``` 

## Kernel Space

The kernel space will be mapped in every Page TLB we use (use the caching
bit?). Each page table structure will contain this for sys enter and what not
that will point to the kernels address space in the upper 3g of memory.

### PDT

PDT (Page Directory Table) will map to itself in the very top end of the
virtual address space (Recursive memory map).  It will hold references to 4KB
Page Tables.

### PTE

PTE (Page Table Entries) will also map to themselfs in the top end of the
virtual address space just below the PTE (Recursive Memory Map). It will hold
references to 4k Pages.

### Kernel Stack

This will be the kernels stack space. Starting at 0xFFBFFFFF and growing down
to 0xFFC00000, the stack will have about 8MB of space.

### Kernel Heap

This is the address range where the kernel can store runtime data structures
like process lists and what not.

### Kernel

This is where our kernel code (including rodata, bss and text) is mapped to.

## User Space

The user space will only exist in a unique page table structure but will be
setup the same in any case.

### Stack

This will be the users stack space. Starting at 0xBFFFFFFF and growing down
to 0xBF800000, the stack will have about 8MB of space.

### Shared Libs

This is where the kernel will map shared libraries in. I do not plan to use
this space in the near future, but this is where it will be when and if i every
implement dynamic linking.

### Heap

This is the address range where the user code can store runtime data like files
and what not.

### Code

This is where the user code (including rodata, bss, and text) will be mapped
to. ELF will identify its section location and it will be obeyed but this is
where the application should be linked to.
