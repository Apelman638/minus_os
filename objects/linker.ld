ENTRY(_start)
SECTIONS
{
    . = 0x100000; /* where the os loads into */
    .text : {
        *(.multiboot)
        *(.text*)
    }
    .rodata : { /* consts */
        *(.rodata*)
    }
    .data : { /* initialized */
        *(.data*)
    }
    .bss : {
        __bss_start = .;
        *(.bss*)         
        *(COMMON)        /* region between __start and end is 0'd out, its all the uninitialized */
        __bss_end = .;   
    }

    . = ALIGN(16); /* aligned the next available memory spot to the 16th byte */
    __heap_start = .; /* heap starts at next available address */
    __heap_end = 0x1000000; /* heap ends here */
}