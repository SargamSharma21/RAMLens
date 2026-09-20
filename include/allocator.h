#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>
#include "memory.h"

int memory_allocate(Block *head , const char *process_name , size_t size);
int memory_free(Block *head, const char *process_name);
void memory_coalesce(Block *head);

#endif