bits 32
global _start
extern kern_main
extern kernel_early

section .multiboot
align 4
    dd 0x1badb002 ; magic number
    dd 0x00 ; flags
    dd -(0x1badb002 + 0x00) ; sum check
    
section .text
_start:
    cli
    mov esp, stack ; moves top of stack to stack pointer
    call kernel_early ; calls the functions in the c file
    call kern_main

section .bss ; saves space
    resb 4096 ; reserving 4096 bytes 

stack: