#include "drivers.h"
#ifndef VGA_H
#define VGA_H

#define VGA_HEIGHT 25
#define VGA_WIDTH  160
#define VGA_MEMORY ((volatile char*)0xB8000)// this is only true for x86



typedef uint16_t vga_char;  
extern int vga_pos;

/*
byte 1 | byte 2
 char    format

byte 2:
4 bits | 4 bits
   bg      tc
*/

typedef enum {
    VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15
} color;

void init_vga();

#endif