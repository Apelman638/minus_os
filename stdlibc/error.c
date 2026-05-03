#include "stdlibc.h"

char error_info[20];

void error(int code) {
    switch (code) {
        case (0) : term_putstr("\tnot implemented command: "); break;
        case (1) : term_putstr("\tunknown command: "); break; 
        case (2) : term_putstr("\tincomplete command: "); break;
        case (3) : term_putstr("\tSave error: unable to save."); break;
        case (4) : term_putstr("\tFunction error: function attempted to parse and read a character not readable."); break;
        case (5) : term_putstr("\tFunction error: memcopy failed due to attempting to access unavailable block number."); break;
        case (6) : term_putstr("\tFunction error: memcopy failed due to attempting to write to unavailable space."); break;
        case (7) : term_putstr("\tFile not found: "); term_putstr(error_info); break; 
        case (8) : term_putstr("\tInput too long."); break;
        case (9) : term_putstr("\tLoad error: unable to load: "); break;
    }
    //term_putstr(term_buffer); // prints the input
    term_putchar('\n');
}

void fatal_error(int code) {
    switch (code) {
        case (0) : {
            term_putstr("\tFATAL ERROR: HEAP NOT INITALIZED: "); 
            //run = 0;
            break;
        }
        case (1) : {
            //run = 0;
        }
    }
}