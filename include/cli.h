#ifndef CLI_H
#define CLI_H

#include "memory.h"
#include "allocator.h"
#include "benchmark.h"

void cli_run(Block **memory, AllocationStrategy *strategy);

#endif