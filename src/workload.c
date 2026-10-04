#include <stdio.h>

#include "workload.h"

MemoryStats workload_run(Block *memory,
                         AllocationStrategy strategy,
                         WorkloadType type,
                         int verbose)
{
    WorkloadOperation basic_workload[] = {
        {WORKLOAD_ALLOC, "Chrome", 200},
        {WORKLOAD_ALLOC, "VSCode", 300},
        {WORKLOAD_FREE,  "Chrome", 0},
        {WORKLOAD_ALLOC, "Firefox", 150},
        {WORKLOAD_ALLOC, "Google", 250},
        {WORKLOAD_FREE,  "VSCode", 0}
    };

    WorkloadOperation fragmentation_workload[] = {
        {WORKLOAD_ALLOC, "A", 100},
        {WORKLOAD_ALLOC, "B", 200},
        {WORKLOAD_ALLOC, "C", 150},
        {WORKLOAD_ALLOC, "D", 250},
        {WORKLOAD_FREE,  "B", 0},
        {WORKLOAD_FREE,  "D", 0},
        {WORKLOAD_ALLOC, "E", 120},
        {WORKLOAD_ALLOC, "F", 180}
    };

    WorkloadOperation stress_workload[] = {
        {WORKLOAD_ALLOC, "P1", 100},
        {WORKLOAD_ALLOC, "P2", 150},
        {WORKLOAD_ALLOC, "P3", 200},
        {WORKLOAD_ALLOC, "P4", 100},
        {WORKLOAD_FREE,  "P2", 0},
        {WORKLOAD_ALLOC, "P5", 120},
        {WORKLOAD_FREE,  "P3", 0},
        {WORKLOAD_ALLOC, "P6", 250},
        {WORKLOAD_FREE,  "P1", 0},
        {WORKLOAD_ALLOC, "P7", 180},
        {WORKLOAD_FREE,  "P4", 0},
        {WORKLOAD_ALLOC, "P8", 100},
        {WORKLOAD_FREE,  "P5", 0},
        {WORKLOAD_ALLOC, "P9", 150}
    };

    WorkloadOperation *workload = NULL;
    size_t count = 0;

    switch (type)
    {
        case WORKLOAD_BASIC:
            workload = basic_workload;
            count = sizeof(basic_workload) /
                    sizeof(basic_workload[0]);
            break;

        case WORKLOAD_FRAGMENTATION:
            workload = fragmentation_workload;
            count = sizeof(fragmentation_workload) /
                    sizeof(fragmentation_workload[0]);
            break;

        case WORKLOAD_STRESS:
            workload = stress_workload;
            count = sizeof(stress_workload) /
                    sizeof(stress_workload[0]);
            break;

        default:
            printf("Unknown workload type.\n");
            return memory_get_stats(memory);
    }

    if (verbose)
    {
        printf("\n========== RUNNING WORKLOAD ==========\n");
    }

    for (size_t i = 0; i < count; i++)
    {
        if (workload[i].type == WORKLOAD_ALLOC)
        {
            if (verbose)
            {
                printf("\nALLOC %s %zu KB\n",
                       workload[i].process_name,
                       workload[i].size);
            }

            if (memory_allocate_with_strategy(
                    memory,
                    workload[i].process_name,
                    workload[i].size,
                    strategy))
            {
                if (verbose)
                {
                    printf("Allocation successful!\n");
                }
            }
            else
            {
                if (verbose)
                {
                    printf("Allocation failed!\n");
                }
            }
        }
        else if (workload[i].type == WORKLOAD_FREE)
        {
            if (verbose)
            {
                printf("\nFREE %s\n",
                       workload[i].process_name);
            }

            if (memory_free(memory, workload[i].process_name))
            {
                if (verbose)
                {
                    printf("Deallocation successful!\n");
                }

                memory_coalesce(memory);
            }
            else
            {
                if (verbose)
                {
                    printf("Deallocation failed!\n");
                }
            }
        }

        if (verbose)
        {
            memory_print(memory);
        }
    }

    if (verbose)
    {
        printf("\n========== WORKLOAD COMPLETE ==========\n");
    }

    return memory_get_stats(memory);
}