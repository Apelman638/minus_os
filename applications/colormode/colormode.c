#include "../../drivers/drivers.h"
#include "../../stdlibc/stdlibc.h"

void color_curser_loc(int loc) { // updates the position of the curser, visually and numerically 
    if(loc < 0) loc = vga_pos; //if it does out of bounds
    vga_pos = loc; // moves pos to loc
} 

void color_extd_keys(uint16_t keycode) { 
    keycode &= 0xff;
    switch(keycode) {
        case (0x48) : color_curser_loc(vga_pos - VGA_WIDTH); break; // up
        case (0x50) : color_curser_loc(vga_pos + VGA_WIDTH); break; // down
        case (0x4b) : color_curser_loc(vga_pos - 2); break; // back
        case (0x4d) : color_curser_loc(vga_pos + 2); break; // forward
    }
}

void colormode() {
    init_vga();
    while(1) {
        uint16_t key = key_reader();
        if(key & 0x100) {
            color_extd_keys(key);
            continue;
        }
        switch(key_map(key)) {
            case 'w' : set_color(vga_pos, ' ', (get_color(vga_pos)>>4) + 5, get_color(vga_pos)); break;
            case 's' : set_color(vga_pos, ' ', (get_color(vga_pos)>>4) - 5, get_color(vga_pos)); break;
            case 'a' : set_color(vga_pos, ' ', (get_color(vga_pos)>>4) - 1, get_color(vga_pos)); break;
            case 'd' : set_color(vga_pos, ' ', (get_color(vga_pos)>>4) + 1, get_color(vga_pos)); break;
            case 0x01 : return; // escape key
        }
    }
}