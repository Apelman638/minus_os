#include "stdlibc.h"

#define LOOPS_PER_SECOND 1000000l

void sleep(double seconds) {
    volatile unsigned long loops = seconds*LOOPS_PER_SECOND;
    while(loops--);
}

//copied from C library
unsigned long seed = 12345;
unsigned long random() {
    seed = seed * 1103515245 + 12345;
    return (seed >> 16) & 0x7FFF;
}