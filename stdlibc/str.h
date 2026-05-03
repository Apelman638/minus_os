#ifndef STR_H
#define STR_H

void strcpy(char* dest, const char* src);
int strlen(const char* str);
int strcmp(const char* str1, const char* str2);
int islower(char ch);
int isupper(char ch);
int isalpha(char ch);
int isdigit(char ch);
int isspace(char ch);
char *lsplit(const char* str, const char split, char* buffer);
char *rsplit(const char* str, const char split, char* buffer);
char *strtok(const char* str, char split, char* buffer);
int to_num(char* str);
void *memcopy(void *dest, const void *src, int n);
int vga_strlen(const vga_char* str);
int n_strcmp(const char* str1, const char* str2, int n);
int vga_strcmp(const void* str1, const void* str2);
int n_vga_strcmp(const void* str1, const void* str2, int n) ;

#endif