#include <stdio.h>
#include <time.h>

#include "benchmark.h"
#include "workload.h"

#define BENCHMARK_RUNS 10000

void benchmark_run(void)
{
    AllocationStrategy strategies[] = {
        FIRST_FIT,
        BEST_FIT,
        WORST_FIT,
        NEXT_FIT
    };

    size_t count = sizeof(strategies) / sizeof(strategies[0]);

    printf("\n========== STRATEGY COMPARISON ==========\n\n");

    printf("%-12s %-12s %-12s %-12s %-18s %-18s %-15s\n",
           "Strategy",
           "Used",
           "Free",
           "Free Blocks",
           "Largest Free",
           "Fragmentation",
           "Time (ms)");

    printf("-----------------------------------------------------------------------------------------------\n");

    for (size_t i = 0; i < count; i++)
    {
        clock_t start = clock();

        for (int run = 0; run < BENCHMARK_RUNS; run++)
        {
            Block *memory = memory_init(1024);

            if (memory == NULL)
            {
                printf("Failed to initialize memory.\n");
                return;
            }

            workload_run(memory, strategies[i], 0);

            memory_destroy(memory);
        }

        clock_t end = clock();

        double total_time =
            ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

        double average_time =
            total_time / BENCHMARK_RUNS;

        /*
         * Run the workload once more to obtain
         * the final memory statistics.
         */
        Block *memory = memory_init(1024);

        if (memory == NULL)
        {
            printf("Failed to initialize memory.\n");
            return;
        }

        MemoryStats stats =
            workload_run(memory, strategies[i], 0);

        memory_destroy(memory);

        printf("%-12s %-12zu %-12zu %-12d %-18zu %-17.2f %-15.6f\n",
               strategy_name(strategies[i]),
               stats.used_memory,
               stats.free_memory,
               stats.free_blocks,
               stats.largest_free_block,
               stats.external_fragmentation,
               average_time);
    }

    printf("-----------------------------------------------------------------------------------------------\n");
    printf("Benchmark runs per strategy: %d\n", BENCHMARK_RUNS);
    printf("==========================================\n");
}