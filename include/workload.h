#ifndef WORKLOAD_H
#define WORKLOAD_H

#include <stddef.h>

#include "memory.h"
#include "allocator.h"

typedef enum {
    WORKLOAD_ALLOC,
    WORKLOAD_FREE
} WorkloadOperationType;

typedef struct {
    WorkloadOperationType type;
    const char *process_name;
    size_t size;
} WorkloadOperation;

MemoryStats workload_run(Block *memory, AllocationStrategy strategy, int verbose);

#endif