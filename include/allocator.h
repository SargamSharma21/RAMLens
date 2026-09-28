#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>
#include "memory.h"

int memory_allocate(Block *head , const char *process_name , size_t size);
int memory_allocate_best_fit(Block *head, const char *process_name, size_t size);
int memory_free(Block *head, const char *process_name);
void memory_coalesce(Block *head);
int memory_allocate_worst_fit(
    Block *head,
    const char *process_name,
    size_t size
);

int memory_allocate_next_fit(
    Block *head,
    const char *process_name,
    size_t size
);

typedef enum
{
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT,
    NEXT_FIT
} AllocationStrategy;

int memory_allocate_with_strategy(
    Block *head,
    const char *process_name,
    size_t size,
    AllocationStrategy strategy
);

const char *strategy_name(AllocationStrategy strategy);

#endif