#include <stdio.h>

#include "workload.h"

void workload_run(Block *memory, AllocationStrategy strategy)
{
    WorkloadOperation workload[] = {
        {WORKLOAD_ALLOC, "Chrome", 200},
        {WORKLOAD_ALLOC, "VSCode", 300},
        {WORKLOAD_FREE,  "Chrome", 0},
        {WORKLOAD_ALLOC, "Firefox", 150},
        {WORKLOAD_ALLOC, "Google", 250},
        {WORKLOAD_FREE,  "VSCode", 0}
    };

    size_t count = sizeof(workload) / sizeof(workload[0]);

    printf("\n========== RUNNING WORKLOAD ==========\n");

    for (size_t i = 0; i < count; i++)
    {
        if (workload[i].type == WORKLOAD_ALLOC)
        {
            printf("\nALLOC %s %zu KB\n",
                   workload[i].process_name,
                   workload[i].size);

            if (memory_allocate_with_strategy(
                    memory,
                    workload[i].process_name,
                    workload[i].size,
                    strategy))
            {
                printf("Allocation successful!\n");
            }
            else
            {
                printf("Allocation failed!\n");
            }
        }
        else if (workload[i].type == WORKLOAD_FREE)
        {
            printf("\nFREE %s\n",
                   workload[i].process_name);

            if (memory_free(memory, workload[i].process_name))
            {
                printf("Deallocation successful!\n");
                memory_coalesce(memory);
            }
            else
            {
                printf("Deallocation failed!\n");
            }
        }

        memory_print(memory);
    }

    printf("\n========== WORKLOAD COMPLETE ==========\n");
}