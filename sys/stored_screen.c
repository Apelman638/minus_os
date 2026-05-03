#include "sys.h"
#include "../master.h"

vga_char stored_vga[VGA_WIDTH*VGA_HEIGHT];
int stored_vga_length;

void save() {
    int i = 0;
    //int j = 0;
    while(i <= vga_pos/2) { // /2 since vga_pos has both char and format position, so it is twice the ammount of vga_char
        //stored_vga[i] = ((VGA_MEMORY[j+1] << 8) & 0xff00) | (VGA_MEMORY[j] & 0x00ff); // stores as vga_char, first byte is the char and 2nd is the format
        memcopy(stored_vga + i, (const vga_char*)(VGA_MEMORY+(i*2)), sizeof(vga_char));
                                // attribute                            char
                                // 0xff - ff
                                // high  low byte
                                // memory is split low byte, high byte in the n_vga_strcmp 
                                // really hard to comprehend and unintuitive at first
        i++;
        //j+=2;
    }
    stored_vga_length = i;
    if(n_vga_strcmp(stored_vga, (const char*)VGA_MEMORY, stored_vga_length*2) == 0) { //uses generic compare to see if whats on the screen is the same as the storage 
        //stored_vga_lenghth holds the amount of vga_char's that are in the stoed_vga, but we are checking it byte by byte 
        // VGA_MEMORY is volatile char*, so it should be cast into const char*
        term_putstr("Save successful.");
        state.loaded = 0;
        return;
    }
    error(3);
    return;  
}

vga_char *load() {
    if (state.loaded == 1) { //this means nothing is saved
        error(9);
        return NULL; 
    }
    state.loaded = 1;
    return stored_vga;
}