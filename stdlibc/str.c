#define DEBUG 0
#if DEBUG
#include <stdio.h>
#else
#include "stdlibc.h"
#endif

int pow(int x, int y) { // x^y, add to its own file
    int ret = x;
    for(int i = 1; i < y; i++) {
        ret *= x;
    }
    return ret;
}

void strcpy(char* dest, const char* src) { //copies the whole string into another and adds a terminator at the end
    while(*src) { // while the char is not null
        *dest = *src; // set the char in this pos to be the src char in the same pos
        dest++;
        src++;
    }
    *dest = '\0';
}

int strlen(const char* str) { // does not include terminator
    int i = 0;
    while(str[i] != 0) {
        i++;
    }
    return i;
}

int vga_strlen(const vga_char* str) { // amount of vga chars are in the string, does not include terminator
    int i = 0;
    while((char)(str[i]>>8) != 0) {
        i++;
    }
    return i*2;
}

int strcmp(const char* str1, const char* str2) { // if equal, returns 0
    while(*str1 == *str2) {
        if(*str1 == '\0') return 0; // means they end at the same time
        str1++, str2++; // progressed through the string
    }
    return 1;
} // works good enough for now

int n_strcmp(const char* str1, const char* str2, int n) { // if equal, returns 0
    while(n-- && (str1++ == str2++));
    if(n == 0) return 0;
    return 1;
} // works good enough for now

int vga_strcmp(const void* str1, const void* str2) { //void* so it can take in VGA_MEMORY
    unsigned char *s1 = (unsigned char*)str1; // splits it byte by byte
    unsigned char *s2 = (unsigned char*)str2;
    while(*s1 == *s2) {
        if(*s1 == '\0') return 0; // means they end at the same time
        s1++, s2++; // progressed through the string
    }
    return 1;
}

int n_vga_strcmp(const void* str1, const void* str2, int n) { //void* so it can take in VGA_MEMORY, which is char
    unsigned char *s1 = (unsigned char*)str1;
    unsigned char *s2 = (unsigned char*)str2;
    while((n--) && (*s2++ == *s1++)); //n-- deincriments n even if it evaluates to 0, so last n is -1 
    // n can be 0 if there is a mismatch on the last character
    if(n < 0) return 0;
    return 1;
} 

void *memcopy(void *dest, const void *src, int n) { // takes in the source byte by byte and sets the dest to it byte by byte
    unsigned char *d = (unsigned char*)dest; //byte by byte
    unsigned char *s = (unsigned char*)src; 

    while(n--) { 
        *d++ = *s++;
    }
    return dest;
}

int islower(char ch) {
    return (ch >= 'a' && ch <= 'z');
}

int isupper(char ch) {
    return (ch >= 'A' && ch <= 'Z');
}

int isalpha(char ch) {
    return (islower(ch) || isupper(ch));
}

int isdigit(char ch) {
    return (ch >= '0' && ch <= '9');
}

int isspace(char ch) {
    return (ch <= 32);
}

char *lsplit(const char* str, const char split, char* buffer) {
    int i = 0; // holds where we are in the string
    while((*str != split) && (*str != '\0')) { // while the string is not terminated and its not the split point
        buffer[i] = *str; //buffer holds the new char
        i++, str++; // moves loc and str pos
    } // if while breaks (terminated, or split found)
    buffer[i] = '\0'; // terminates buffer
    return buffer;
}

char *rsplit(const char* str, const char split, char* buffer) { 
    int startlength = strlen(str); // starts at the end of the string, strlen does not include null terminator
    int i = startlength; //hold the loc we are at
    while((i >= 0) && (str[i] != split)) { // while we are within the string, anda string is not split
        i--; // continue moving down
    }

    int j = 0; // holds the loc from the start of the split
    while(str[i + j + 1] != '\0') {  //starts from the position the split is at 
        buffer[j] = str[i + j]; // adds it to the buffer and moves up
        j++;
    }
    buffer[j] = '\0'; // end terminates
    return buffer;
}

char *to_lower(char *src, char *buffer) {
    if(isupper(*src) == 0) return 0;
    while(*src != '\0') {
        *buffer = *src-36; //down 36 is the lower chars
        src++; //moves up pointer
    }
    return buffer;
}

char *to_upper(char *src, char *buffer) {
    if(islower(*src) == 0) return 0;
    while(*src != '\0'){
        *buffer = *src+36; 
        src++; // moves pointer
    }
    return buffer;
}

int to_num(char* str) {
    int i = 0;
    int ret = 0;
    while(*(str+i) != '\0') {
        if(isspace(*(str+i))) {
            i++;
            continue;
        }
        if(!isdigit(*(str+i))) {
            #if !DEBUG
            error(4);
            #endif
        }
        switch (*(str+i)) {
            case ('0') : ret += 0; break;
            case ('1') : ret += pow(10,(strlen(str)-i-1)); break;
            case ('2') : ret += 2*pow(10,(strlen(str)-i-1)); break;
            case ('3') : ret += 3*pow(10,(strlen(str)-i-1)); break;
            case ('4') : ret += 4*pow(10,(strlen(str)-i-1)); break;
            case ('5') : ret += 5*pow(10,(strlen(str)-i-1)); break;
            case ('6') : ret += 6*pow(10,(strlen(str)-i-1)); break;
            case ('7') : ret += 7*pow(10,(strlen(str)-i-1)); break;
            case ('8') : ret += 8*pow(10,(strlen(str)-i-1)); break;
            case ('9') : ret += 9*pow(10,(strlen(str)-i-1)); break;
            default: return 0;
        }
        i++;
    }
    return ret;
}

int is_operation(char ch) {
    switch (ch) {
        case '+' : return 1;
        case '-' : return 1;
        case '/' : return 1;
        case '*' : return 1;
        case '^' : return 1;
        default: return 0;
    }
}

char *strtok(const char* str, char split, char* buffer) {
    static int ptr; // hold where in the string we were left off
    static char saved_str[100]; //malloc would sure be nice, not no stdlib
    int buff_loc = 0; // where in the buffer we are putting the string
    if(str != NULL) { // if it is null we are continuing the last string
        ptr = 0; // if its not, we are doing a new string
        strcpy(saved_str, str);
    } 
    while((*(saved_str + ptr) != split) && (*(saved_str + ptr) != '\0')) {
        *(buffer + buff_loc) = *(saved_str + ptr);
        ptr++;
        buff_loc++;
    }
    ptr++; // if we are going over a string again, the ptr was last left off at the split point
    *(buffer + buff_loc) = '\0'; // ends string
    return buffer;
}

#if DEBUG
int main() {
    char buffer[100];
    char* test = "HELLO WORLD";
    printf("%s\n", strtok(test, ' ', buffer));
    strcpy(buffer, "");
    printf("%s\n", strtok(NULL, ' ', buffer));
    return 0;
}
#endif