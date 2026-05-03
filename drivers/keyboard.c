// this will be used to take in user input from the keyboard
#include "drivers.h" //for uint8_t etc
#define NULL 0

#define READPORT (volatile uint8_t)0x64
#define INPUTPORT (volatile uint8_t)0x60
#define NO 0
#define DEBUG 1

/*
typedef struct {
    unsigned int write : 1;
    unsigned int shift : 1;
    unsigned int loaded : 1;
    unsigned int color : 1;
} Flags; 
*/

static inline uint8_t inb(uint16_t port) {
    int val;
    __asm__ volatile ("inb %%dx, %%al" : "=a"(val) : "d"(port));
    /*
    %1 is the seconds parameter (port)
    %0 is the first =a

    this puts the value returned by the port and puts it into val
    */
    return val;
}

static inline void outb(uint16_t port, int val) {
    __asm__ volatile ("outb %0, %1" :: "a"(val), "Nd"(port));
    /*
    outputs val into port
    */
}

//create key reader
uint16_t key_reader() { // takes input from the ports, returns keycode for it
    while(!(inb(READPORT) & 0x01)); // checks if the first bit of the readport is on, if not it continues to wait
    uint8_t key = inb(INPUTPORT);
    // extended keyboard
    if(key == 0xe0) { // checks if its an extended character 
        while(!(inb(READPORT) & 0x01)); // waits for 2nd byte
        return inb(INPUTPORT) | 0x100;
    }
    if (key == 0x2a || key == 0x36) { // if the left and right shift have been pressed
        state.shift = 1; // this is true
        return NULL; // returns nothing so it can do keyshift instead
    }
    if (key == 0xaa || key == 0xb6) { // if shift released
        state.shift = 0;
        return NULL; // no character is returned, just shift held is disabled
    }
    return key;
}

const char keys[] = {
    NO, 0x01, '1', '2', '3', '4', '5', '6', '7','8','9','0',
    '-','=','\b','\t','q','w','e','r','t','y','u','i','o',
    'p','[',']','\n',NO,'a','s','d','f','g','h','j','k','l',
    ';','\'',NO,NO,'\\','z','x','c','v','b','n','m',',','.',
    '/', NO,NO,NO,' '
}; 
const char shifted_keys[] = {
    NO, NO, '!', '@', '#', '$', '%', '^', '&','*','(',')',
    '_','+','\b','\t','Q','W','E','R','T','Y','U','I','O',
    'P','{','}','\n', NO,'A','S','D','F','G','H','J','K','L',
    ':','\"' ,NO,NO,'|','Z','X','C','V','B','N','M','<','>',
    '?', NO,NO,NO,' '
}; 

void extd_keys(uint16_t keycode) { 
    keycode &= 0xff;
    switch(keycode) {
        case (0x48) : set_color(vga_pos, VGA_MEMORY[vga_pos], 0, VGA_COLOR_LIGHT_GREY); curser_loc(vga_pos - VGA_WIDTH); break; // up
        case (0x50) : set_color(vga_pos, VGA_MEMORY[vga_pos], 0, VGA_COLOR_LIGHT_GREY); curser_loc(vga_pos + VGA_WIDTH); break; // down
        case (0x4b) : set_color(vga_pos, VGA_MEMORY[vga_pos], 0, VGA_COLOR_LIGHT_GREY); curser_loc(vga_pos - 2); break; // back
        case (0x4d) : set_color(vga_pos, VGA_MEMORY[vga_pos], 0, VGA_COLOR_LIGHT_GREY); curser_loc(vga_pos + 2); break; // forward
    }
}

char key_map(uint16_t keycode) { // takes keycode, returns char
    if((keycode & 0x100) == 0x100) {
        extd_keys(keycode); 
        return NULL;
    }
    if(keycode > sizeof(shifted_keys)) return 0; // out of bounds
    if(state.shift) return shifted_keys[keycode]; // shifted key
    return keys[keycode]; // regular key
}