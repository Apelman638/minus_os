#ifndef KEYBOARD_H
#define KEYBOARD_H

#define READPORT (volatile uint8_t)0x64
#define INPUTPORT (volatile uint8_t)0x60
#define NO 0

uint8_t inb(uint16_t port);

extern char term_buffer[100];
extern const char keys[];
extern int random_factor;

uint16_t key_reader();
void extd_keys(uint16_t keycode);
char key_map(uint16_t keycode);

#endif