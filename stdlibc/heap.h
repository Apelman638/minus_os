#ifndef HEAP_H
#define HEAP_H

void init_heap(int size, void* start);
void *malloc(int size);
void free(void* ptr);

#endif