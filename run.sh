#!/bin/bash
#not a fan of make so i used this

nasm -f elf32 bootloader.s -o objects/boot.o 
x86_64-elf-gcc -c drivers/vga.c -o objects/vga.o -ffreestanding -nostdlib -m32
x86_64-elf-gcc -c drivers/scr.c -o objects/scr.o -ffreestanding -nostdlib -m32
x86_64-elf-gcc -c stdlibc/error.c -o objects/error.o -ffreestanding -nostdlib -m32
x86_64-elf-gcc -c stdlibc/str.c -o objects/str.o -ffreestanding -nostdlib -m32
x86_64-elf-gcc -c drivers/keyboard.c -o objects/keyboard.o -ffreestanding -nostdlib -m32
x86_64-elf-gcc -c stdlibc/heap.c -o objects/heap.o -ffreestanding -nostdlib -m32
x86_64-elf-gcc -c stdlibc/super.c -o objects/super.o -ffreestanding -nostdlib -m32
x86_64-elf-gcc -c sys/files.c -o objects/files.o -ffreestanding -nostdlib -m32
x86_64-elf-gcc -c sys/ramdisk.c -o objects/ramdisk.o -ffreestanding -nostdlib -m32
x86_64-elf-gcc -c sys/stored_screen.c -o objects/stored_screen.o -ffreestanding -nostdlib -m32
x86_64-elf-gcc -c shell/shell.c -o objects/shell.o -ffreestanding -nostdlib -m32

x86_64-elf-gcc -c applications/snake/snake.c -o objects/snake.o -ffreestanding -nostdlib -m32
x86_64-elf-gcc -c applications/snake/snake_special_functions.c -o objects/snake_special_functions.o -ffreestanding -nostdlib -m32


x86_64-elf-gcc -c kernel.c -o objects/kernel.o -ffreestanding -nostdlib -m32
x86_64-elf-ld -m elf_i386 -T linker.ld objects/shell.o objects/ramdisk.o objects/stored_screen.o objects/boot.o objects/snake.o objects/str.o objects/kernel.o objects/scr.o objects/vga.o objects/keyboard.o objects/error.o objects/heap.o objects/snake_special_functions.o objects/files.o objects/super.o -o objects/kernel.elf
qemu-system-x86_64 -machine pc -kernel objects/kernel.elf       