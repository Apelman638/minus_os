#include "../master.h"

void write_file(const char* input_name, const vga_char* contents) {
    // loads the inodes and superblock
    inode table[INODE_COUNT]; // loads all of these structs 
    ram_read(1, table, BLOCK_SIZE); 
    superblock info; // loads the super blocks
    ram_read(0, &info, sizeof(superblock)); // loads only the superblock into the struct
    
    int write_to_pos = info.new_file_ptr; // takes the recent new file ptr from the superblock
    memcopy(table[write_to_pos - 2].filename, input_name, strlen(input_name) + 1); // copy filename with terminator
    table[write_to_pos - 2].disk_loc = write_to_pos; // updates the position the file is it
    table[write_to_pos - 2].size = vga_strlen(contents)/2; // /2 since it gets *2 in readfile

    ram_write(1, table, BLOCK_SIZE); // updates the inode list
    ram_write(info.new_file_ptr, contents, vga_strlen(contents)); // loads the new file into the position
    info.new_file_ptr++; // updates the new file ptr
    ram_write(0, &info, sizeof(superblock)); // updates the superblock
}

vga_char *read_file(const char* filename, vga_char* buffer) { 
    if(strlen(filename) > 20) {
        error(8);
        return NULL;
    }
    int pos;
    inode table[INODE_COUNT]; // loads all of these structs 
    ram_read(1, table, BLOCK_SIZE); // reads all the structs into the table
    int found = 0; // if the file is found
    int file_size;
    for(int i = 0; i < INODE_COUNT; i++) {
        if(strcmp(filename, table[i].filename) == 0) {
            pos = table[i].disk_loc;
            found = 1;
            file_size = table[i].size; // automattically reads justt the size of the file into the buffer
            break;
        }
    }
    if (found == 0) {
        memcopy(error_info, filename, 20);
        error(7);
        return NULL;
    }
    ram_read(pos, buffer, file_size*2); //times 2 since the type is vga char
    return buffer;
}

char** ls(char* buffer[]) { // returns all files
    inode table[INODE_COUNT]; // loads all of the structs 
    ram_read(1, table, BLOCK_SIZE); // reads all the structs into the table
    superblock sb;
    ram_read(0, &sb, sizeof(superblock));
    int file_count = sb.new_file_ptr - 2;
    int i = 0;
    while(i < file_count) { // goes until the filename no longer exists
        (buffer[i]) = table[i].filename; // probably the error source, the string in i is set to the filename 
        i++;
    }
    buffer[i] = '\0';
    return buffer; 
}