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

typedef struct {
    size_t total_memory;
    size_t used_memory;
    size_t free_memory;

    int allocated_blocks;
    int free_blocks;
    double external_fragmentation;
    size_t largest_free_block;
} MemoryStats;

MemoryStats memory_get_stats(Block *head);

Block *memory_init(size_t size);
void memory_destroy(Block *head);
void memory_print(Block *head);
int memory_allocate(Block *head , const char *process_name , size_t size);
int memory_free(Block *head, const char *process_name);

#endif