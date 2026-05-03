#include "../../drivers/drivers.h"
#include "../../stdlibc/stdlibc.h"

uint16_t no_yield_key_reader() { // takes input from the ports, returns keycode for it. does not wait for input
    uint8_t key = NULL;
    if((inb(READPORT) & 0x01)) {
        key = inb(INPUTPORT);
    }
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

uint16_t no_yield_arrow_reader() { // version specifically for arrow keys
    // for snake game
    uint8_t key = NULL;
    if((inb(READPORT) & 0x01)) {
        key = inb(INPUTPORT);
    }
    // extended keyboard
    if(key == 0xe0) { // checks if its an extended character 
        while(!(inb(READPORT) & 0x01)); // waits for 2nd byte
        return inb(INPUTPORT) | 0x100;
    }
    return key;
}

int key_wait_arrow() { // for snake game
    int try_key_read;
    int ret_key = 0;
    for(volatile int i = 0; i < 3000000; i++) {
        try_key_read = no_yield_arrow_reader();
        if (try_key_read & 0x100) {
            ret_key = try_key_read;
            random_factor += try_key_read; // uses every input here to help determine how the random function will randomize.
            random_factor &= 0xffffffff;
        }
    }
    return ret_key;
}