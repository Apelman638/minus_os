#ifndef SCR_H
#define SCR_H
#include "drivers.h"
#define ROW_DOWN ((vga_pos + VGA_WIDTH) - vga_pos%(VGA_WIDTH))

typedef struct {
    unsigned int write : 1;
    unsigned int shift : 1;
    unsigned int loaded : 1;
    unsigned int color : 1;
    unsigned int write_file : 1;
} Flags; 

extern Flags state;

void term_putchar(char ch);
void curser_loc(int loc);
void term_putstr(const char* str);
void set_color(int loc, char ch, color bg, color tc);
vga_char *buff_put_formatted_str(vga_char* buffer, const char* str, color bg, color tc);
void term_put_formatted_char(char ch, color bg, color tc);
void term_put_formatted_str(const char* str, color bg, color tc);
void put_vga_char(const vga_char ch);
void put_vga_str(const vga_char* str);

#endif