#include "drivers.h"
#define ROW_DOWN ((vga_pos + VGA_WIDTH) - vga_pos%(VGA_WIDTH))

Flags state = {0,0,1,0,0};

void curser_loc(int loc) { // updates the position of the curser, visually and numerically 
    if(loc < 0) loc = vga_pos; //if it does out of bounds
    vga_pos = loc; // moves pos to loc
    VGA_MEMORY[loc+1] = VGA_COLOR_DARK_GREY << 4 | VGA_COLOR_LIGHT_GREY; // changes text color there to be right
    if(((uintptr_t)VGA_MEMORY+loc) > (uintptr_t)0xb8000) { // if within bounds
        VGA_MEMORY[loc-1] = VGA_COLOR_BLACK << 4 | VGA_COLOR_LIGHT_GREY; // sets the former position to be not highlighted
    }
} 

void set_color(int loc, char ch, color bg, color tc) {
    VGA_MEMORY[loc] = ch; //puts char in that spot
    VGA_MEMORY[loc+1] = bg << 4 | tc; //sets format to bg and tc
}

void term_put_formatted_char(char ch, color bg, color tc) {
    if(ch == '\n') { //new line
        VGA_MEMORY[vga_pos+1] = bg << 4 | tc; //sets the format for the current curser pos to not be highlighted
        VGA_MEMORY[vga_pos-1] = bg << 4 | tc; //sets the format for the spot before curser pos to not be highlighted
        // doesnt work for some reason without both 
        vga_pos = ROW_DOWN; // moves down to beginning of line
        curser_loc(vga_pos); // sets the curser to new spot
        return;
    }
    if(ch == '\r') { // returns to beginning of line
        VGA_MEMORY[vga_pos-1] = bg << 4 | tc; //sets the format right before the curser to not be highlighted
        vga_pos = vga_pos - (vga_pos%VGA_WIDTH); // beginning of line, finds how far it is into the line and moves it back
        curser_loc(vga_pos); 
        return;
    }
    if(ch == '\t') { //tab
        VGA_MEMORY[vga_pos+1] = bg << 4 | tc; //not sure why works
        vga_pos += 8; //4 spaces
        curser_loc(vga_pos);
        return;
    }
    if(ch == '\b') { //delete
        if(vga_pos - 2 < 0) return;
        VGA_MEMORY[vga_pos] = ' ';  //sets current pos to be nothing
        VGA_MEMORY[vga_pos+1] = bg << 4 | tc; // since it goes back, it needs to move the curser back by setting the spot to not be highlighted
        vga_pos-=2; // moves back 1 spot
        curser_loc(vga_pos); // resets vga pos
        return;
    }
    VGA_MEMORY[vga_pos] = ch; // prints if normal char 
    vga_pos+=2; // 2 since there is char bit then format
    curser_loc(vga_pos);
}

void term_putchar(char ch) {
    if(ch == '\n') { //new line
        VGA_MEMORY[vga_pos+1] = VGA_COLOR_BLACK << 4 | VGA_COLOR_LIGHT_GREY; //sets the format for the current curser pos to not be highlighted
        VGA_MEMORY[vga_pos-1] = VGA_COLOR_BLACK << 4 | VGA_COLOR_LIGHT_GREY; //sets the format for the spot before curser pos to not be highlighted
        // doesnt work for some reason without both 
        vga_pos = ROW_DOWN; // moves down to beginning of line
        curser_loc(vga_pos); // sets the curser to new spot
        return;
    }
    if(ch == '\r') { // returns to beginning of line
        VGA_MEMORY[vga_pos-1] = VGA_COLOR_BLACK << 4 | VGA_COLOR_LIGHT_GREY; //sets the format right before the curser to not be highlighted
        vga_pos = vga_pos - (vga_pos%VGA_WIDTH); // beginning of line, finds how far it is into the line and moves it back
        curser_loc(vga_pos); 
        return;
    }
    if(ch == '\t') { //tab
        VGA_MEMORY[vga_pos+1] = VGA_COLOR_BLACK << 4 | VGA_COLOR_LIGHT_GREY; //not sure why works
        vga_pos += 8; //4 spaces
        curser_loc(vga_pos);
        return;
    }
    if(ch == '\b') { //delete
        if(vga_pos - 2 < 0) return;
        VGA_MEMORY[vga_pos-2] = ' ';  //resets previous position, that is being deleted
        VGA_MEMORY[vga_pos+1] = VGA_COLOR_BLACK << 4 | VGA_COLOR_LIGHT_GREY; // clears curser from this position
        vga_pos-=2; // moves back 1 spot
        curser_loc(vga_pos); // resets vga pos, puts curser at that spot
        return;
    }
    VGA_MEMORY[vga_pos] = ch; // prints if normal char 
    vga_pos+=2; // 2 since there is char bit then format
    curser_loc(vga_pos);
}

void term_putstr(const char* str) {
    int i = 0;
    while(str[i] != '\0') {
        term_putchar(str[i]);
        i++;
    }
}

void term_put_formatted_str(const char* str, color bg, color tc) {
    int i = 0;
    while(str[i] != '\0') {
        term_put_formatted_char(str[i], bg, tc);
        i++;
    }
}

vga_char *buff_put_formatted_str(vga_char* buffer, const char* str, color bg, color tc) { // converts string into vga string
    while(*str) {
        *buffer = (*str << 8) | ((bg << 4) | tc); // first byte is the char, 2nd is format
        buffer++, str++;
    }
    return buffer; 
}

void put_vga_char(const vga_char ch_in) {
    unsigned char format = ch_in & 0x00FF; // takes the first byte
    char ch = (ch_in & 0xff00) >> 8; // takes the character byte, then puts it into a character
    if(ch == '\n') { //new line
        VGA_MEMORY[vga_pos+1] = format; //sets the format for the current curser pos to not be highlighted
        VGA_MEMORY[vga_pos-1] = format; //sets the format for the spot before curser pos to not be highlighted
        // doesnt work for some reason without both 
        vga_pos = ROW_DOWN; // moves down to beginning of line
        curser_loc(vga_pos); // sets the curser to new spot
        return;
    }
    if(ch == '\r') { // returns to beginning of line
        VGA_MEMORY[vga_pos-1] = format; //sets the format right before the curser to not be highlighted
        vga_pos = vga_pos - (vga_pos%VGA_WIDTH); // beginning of line, finds how far it is into the line and moves it back
        curser_loc(vga_pos); 
        return;
    }
    if(ch == '\t') { //tab
        VGA_MEMORY[vga_pos+1] = format; //not sure why works
        vga_pos += 8; //4 spaces
        curser_loc(vga_pos);
        return;
    }
    if(ch == '\b') { //delete
        if(vga_pos - 2 < 0) return;
        VGA_MEMORY[vga_pos] = ' ';  //sets current pos to be nothing
        VGA_MEMORY[vga_pos+1] = format; // since it goes back, it needs to move the curser back by setting the spot to not be highlighted
        vga_pos-=2; // moves back 1 spot
        curser_loc(vga_pos); // resets vga pos
        return;
    }
    VGA_MEMORY[vga_pos] = ch; // prints if normal char 
    VGA_MEMORY[vga_pos+1] = format;
    vga_pos+=2; // 2 since there is char bit then format
    curser_loc(vga_pos);
}

void put_vga_str(const vga_char* str) {
    int i = 0;
    while((char)(str[i]>>8) != '\0') {
        put_vga_char(str[i]);
        i++;
    }
}