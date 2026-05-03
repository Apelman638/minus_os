#include "stdlibc.h"

typedef struct heap_block {
    int size;
    int free;
    struct heap_block *next;
} heap_header;

heap_header* head = NULL; 

void init_heap(int size, void* start) {
    head = (heap_header*)start; // puts the heap header in the mem address of start
    head->size = size - sizeof(heap_header); // this is the total size of the heap
    head->next = NULL;
}

void *malloc(int size) {
    heap_header* current = head; // id want to make this static 
    if(!head) { // heap is not initalized
        fatal_error(0);
        return NULL;
    }
    if (size < 1) {
        return NULL;
    }

    while(current) {
        if((current->size >= size) && current->free) { // if the size is good and the block is free
            heap_header* new_block = (heap_header*)((uint8_t*)(current) + sizeof(heap_header) + size); // moves up to the position current is at, then moves up the size of the header up to the size of the memory being allocated
            new_block->free = 1; // the block is marked as free, as it is the next space that memory will be allocated to
            new_block->next = NULL; // will be set once the block is taken
            // the size of the first block is the size of the heap
            new_block->size = current->size - sizeof(heap_header) - size; // this is the remaining size left in heap

            current->free = 0; // current block is no longer free memory
            current->next = new_block; 
            current->size = size; // this is the size taken up by the malloc

            return (void*)((uint8_t*)current + sizeof(heap_header)); // returns address of the allocated space
        }
        current = current->next; // this causes it to loop through the heap to find all the structs that are linked
    }
    return NULL; // no space is found
}

void free(void* ptr) {
    heap_header* ptr_head = (heap_header*)(ptr - sizeof(heap_header)); // finds the memory address of this ptr's header
    ptr_head->free = 1; // marks it as free, so as long as another space of a lower size is needed itll take it 
}