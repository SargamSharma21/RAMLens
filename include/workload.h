#ifndef WORKLOAD_H
#define WORKLOAD_H

#include <stddef.h>

#include "memory.h"
#include "allocator.h"

typedef enum {
    WORKLOAD_ALLOC,
    WORKLOAD_FREE
} WorkloadOperationType;

typedef enum {
    WORKLOAD_BASIC,
    WORKLOAD_FRAGMENTATION,
    WORKLOAD_STRESS
} WorkloadType;

typedef struct {
    WorkloadOperationType type;
    const char *process_name;
    size_t size;
} WorkloadOperation;

MemoryStats workload_run(Block *memory, AllocationStrategy strategy, WorkloadType type, int verbose);

#endif