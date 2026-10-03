#include <stdio.h>

#include "benchmark.h"
#include "workload.h"

void benchmark_run(void)
{
    AllocationStrategy strategies[] = {
        FIRST_FIT,
        BEST_FIT,
        WORST_FIT,
        NEXT_FIT
    };

    size_t count = sizeof(strategies) / sizeof(strategies[0]);

    printf("\n========== STRATEGY COMPARISON ==========\n");

    for (size_t i = 0; i < count; i++)
    {
        Block *memory = memory_init(1024);

        if (memory == NULL)
        {
            printf("Failed to initialize memory.\n");
            return;
        }

        printf("\nRunning: %s\n", strategy_name(strategies[i]));

        MemoryStats stats =
            workload_run(memory, strategies[i] , 0);

        printf("%-12s %-12zu %-12zu %-12d %-18zu %-17.2f%%\n",
        strategy_name(strategies[i]),
        stats.used_memory,
        stats.free_memory,
        stats.free_blocks,
        stats.largest_free_block,
        stats.external_fragmentation);

        memory_destroy(memory);
    }

    printf("\n==========================================\n");
}