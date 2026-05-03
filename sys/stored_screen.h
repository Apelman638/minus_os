#ifndef STORED_SCREEN_H
#define STORED_SCREEN_H
#include "../master.h"

extern vga_char stored_vga[VGA_WIDTH*VGA_HEIGHT];
vga_char* load();
void save();
extern int stored_vga_length;
#endif 