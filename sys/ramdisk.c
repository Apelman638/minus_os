#include "sys.h"
#include "../master.h"

static uint8_t ramdisk[BLOCK_SIZE*BLOCK_COUNT]; // this section of memory is reserved by bss

void ram_write(int block_num, const void *info, int size) { 
    if(size > BLOCK_SIZE || block_num >= BLOCK_COUNT || block_num < 0) {
        error(6);
        return;
    }
    memcopy(ramdisk + block_num*BLOCK_SIZE, info, size);
}

void ram_read(int block_num, void* buff, int size) {
    if(block_num > BLOCK_COUNT || block_num < 0) {
        error(5); 
        return;
    }
    memcopy(buff, ramdisk + block_num*BLOCK_SIZE, size);
    // copies into the buffer, the block at that points
}

/*
superblock at 0: entrance to file system
inodes at 1: list of file information
files at 2+: 
*/

superblock sb = {0xDEADC0DE, BLOCK_COUNT, BLOCK_COUNT-2, 2}; // text file
inode total_files[BLOCK_SIZE/INODE_SIZE];