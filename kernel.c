#include "master.h"

extern uint8_t __heap_start;
extern uint8_t __heap_end;
extern uint8_t __bss_start;
extern uint8_t __bss_end;

void kernel_early(void) {
    init_vga();
    init_heap(&__heap_start - &__heap_end, &__heap_start);
    ram_write(0, &sb, sizeof(sb)); // super block for defining the start of the file system, at spot 0
    ram_write(1, total_files, BLOCK_SIZE); // puts the list of total files into block 1
    char* test = "Hello world, \nThis is file 1 and it lives on this operating system!!!";
    vga_char buffer[strlen(test)];
    buff_put_formatted_str(buffer, test, VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY); //format string into a vga_char
    write_file("testfile", buffer);
    // creates file 1.
}

int kern_main(void) {
    start_up:
    shell(); // runs until exited
    init_vga(); // clears vga
    curser_loc(0); // resets curser position
    term_putstr("exit success.");
    while(1);
    return 0;
}
