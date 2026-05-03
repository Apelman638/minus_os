#include "../../drivers/drivers.h"
#include "../../stdlibc/stdlibc.h"

//add to its own file.
uint16_t no_yield_key_reader() { // takes input from the ports, returns keycode for it. does not wait for input
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
            random_factor += try_key_read;
            random_factor &= 0xffffffff;
        }
    }
    return ret_key;
}

int running = 1;
int dir_reload = 0x48;

typedef struct {
    int x;
    int y;
} Coord;

Coord snake_parts[VGA_HEIGHT * VGA_WIDTH/2]; //diveded by 2 because other wise its amount of bytes, not pixels
int length = 1;

Coord apple = {10, 10};

void init_snake() {
    snake_parts[0].x = 80; // 80th byte
    snake_parts[0].y = 12;
}

int is_on_snake(Coord* obj) {
    for(int i = 1; i < length; i++) {
        if(obj->x == snake_parts[i].x && obj->y == snake_parts[i].y) {
            return 1;
        }
    }
    return 0;
}

void display_snake() {
    for(int i = 0; i < length; i++) {
        int loc = (snake_parts[i].x + snake_parts[i].y*VGA_WIDTH);
        VGA_MEMORY[loc] = 0xDB;
        VGA_MEMORY[loc+1] = VGA_COLOR_LIGHT_GREY; 
    }
    int apple_loc = (apple.x + apple.y*VGA_WIDTH);
    VGA_MEMORY[apple_loc] = 'O';
    VGA_MEMORY[apple_loc+1] = VGA_COLOR_RED; 
}

void death() {
    running = 0;
}

void move_x(int dir) {
    if(snake_parts[0].x+dir*2 > VGA_WIDTH-1 || snake_parts[0].x+dir*2 < 0) {
        death();
        return;
    }
    for(int i = length - 1; i > 0; i--) {
        snake_parts[i].x = snake_parts[i-1].x;
        snake_parts[i].y = snake_parts[i-1].y;
    }
    snake_parts[0].x += dir*2;
    display_snake();
}

void move_y(int dir) {
    if(snake_parts[0].y-dir > VGA_HEIGHT-1 || snake_parts[0].y-dir < -1) {
        death();
        return;
    }
    for(int i = length - 1; i > 0; i--) {
        snake_parts[i].x = snake_parts[i-1].x;
        snake_parts[i].y = snake_parts[i-1].y;
    }
    snake_parts[0].y -= dir;
    display_snake();
}

void check_pos() {
    if(is_on_snake(&snake_parts[0])) {
        death();
    }
    if(apple.x == snake_parts[0].x && apple.y == snake_parts[0].y) {
        int tail_x = 100;
        int tail_y = 26;
        
        length++;

        snake_parts[length-1].x = tail_x-2;
        snake_parts[length-1].y = tail_y-1;

        do {
            apple.x = (random() % VGA_WIDTH/2)*2;
            apple.y = random() % VGA_HEIGHT;
        } while(is_on_snake(&apple));
    }
}

void snake() {
    running = 1;
    dir_reload = 0x48;
    length = 1;
    init_vga();
    init_snake();
    display_snake();
    while(running) {
        display_snake();
        int key_code = key_wait_arrow();
        if(key_code == 0) {
            key_code = dir_reload;
        }
        key_code &= 0xff;
        switch(key_code) {
            case (0x48) : {
                move_y(1); 
                dir_reload = 0x48;
                break; // up
            }
            case (0x50) : {
                move_y(-1); 
                dir_reload = 0x50;
                break; // down
            }
            case (0x4b) : {
                move_x(-1); 
                dir_reload = 0x4b;
                break; // back
            }
            case (0x4d) : {
                move_x(1); 
                dir_reload = 0x4d;
                break; // forward
            }
        }
        check_pos();
        init_vga();
    }
}