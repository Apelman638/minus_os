#ifndef RAMDISK_H
#define RAMDISK_H

#define BLOCK_SIZE 2048 //1024 total chars since the screen has formatting and char bytes
#define BLOCK_COUNT 2048

void ram_write(int block_num, const void *info, int size);
void ram_read(int block_num, void* buff, int size);

typedef struct {
    int magic; 
    int total_blocks;
    int free_blocks;
    int new_file_ptr;
} superblock;  

typedef struct {
    char filename[20]; // 20 bytes
    int disk_loc; // 4 bytes, points to the block its stored at
    short int size; // 2 bytes
} inode; // file metadata, a list of these will be stored into block 1

#define INODE_SIZE sizeof(inode)
#define INODE_COUNT BLOCK_SIZE/INODE_SIZE

extern superblock sb;
extern inode total_files[INODE_COUNT]; //idek how many
// at the end of the inode limit, add a new superblock ?? maybe for a different type of file

#endif