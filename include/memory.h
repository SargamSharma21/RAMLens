#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

typedef struct Block {
    size_t start;
    size_t size;
    int is_free;
    char process_name[32];

    struct Block *next;
} Block;

Block *memory_init(size_t size);
void memory_destroy(Block *head);

#endif