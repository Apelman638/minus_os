#ifndef FILES_H
#define FILES_H
#include "../master.h"

void write_file(const char* input_name, const vga_char* contents);
vga_char *read_file(const char* filename, vga_char* buffer);
char** ls(char** buffer);

#endif