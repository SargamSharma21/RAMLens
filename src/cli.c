#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "workload.h"
#include "cli.h"

void cli_run(Block *memory, AllocationStrategy *strategy)
{
    char command[100];

    while (1)
    {
        printf("RAMLens > ");

        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            break;
        }

        command[strcspn(command, "\n")] = '\0';

        char *token = strtok(command, " ");

        if (token == NULL)
        {
            continue;
        }

        if (strcmp(token, "exit") == 0)
        {
            break;
        }

        if (strcmp(token, "alloc") == 0)
        {
            char *process_name = strtok(NULL, " ");
            char *size_string = strtok(NULL, " ");

            if (process_name == NULL || size_string == NULL)
            {
                printf("Usage: alloc <process> <size>\n");
                continue;
            }

            size_t size = atoi(size_string);

            if (memory_allocate_with_strategy(
                    memory,
                    process_name,
                    size,
                    *strategy))
            {
                printf("Allocation successful!\n");
            }
            else
            {
                printf("Allocation failed!\n");

                MemoryStats stats = memory_get_stats(memory);

                printf("\n========== ALLOCATION FAILURE ==========\n");
                printf("Requested Memory   : %zu KB\n", size);
                printf("Total Free Memory : %zu KB\n", stats.free_memory);
                printf("Largest Free Block: %zu KB\n",
                    stats.largest_free_block);

                if (stats.free_memory < size)
                {
                    printf("Reason             : Insufficient free memory.\n");
                }
                else if (stats.largest_free_block < size)
                {
                    printf("Reason             : External fragmentation.\n");
                }

                printf("=========================================\n");
            }

            memory_print(memory);
        }
        else if (strcmp(token, "free") == 0)
        {
            char *process_name = strtok(NULL, " ");

            if (process_name == NULL)
            {
                printf("Usage: free <process>\n");
                continue;
            }

            if (memory_free(memory, process_name))
            {
                printf("Deallocation successful!\n");
                memory_coalesce(memory);
            }
            else
            {
                printf("Deallocation failed!\n");
            }

            memory_print(memory);
        }
        else if (strcmp(token, "workload") == 0)
        {
            workload_run(memory, *strategy);
        }
        else if (strcmp(token, "map") == 0)
        {
            memory_print(memory);
        }
        
        else if (strcmp(token, "stats") == 0)
        {
            MemoryStats stats = memory_get_stats(memory);
            double utilization = 0.0;

            if (stats.total_memory > 0)
            {
                utilization =
                    ((double)stats.used_memory / stats.total_memory) * 100.0;
            }
            printf("\n========== MEMORY STATISTICS ==========\n");
            printf("Total Memory           : %zu KB\n", stats.total_memory);
            printf("Used Memory            : %zu KB\n", stats.used_memory);
            printf("Free Memory            : %zu KB\n", stats.free_memory);
            printf("Allocated Blocks       : %d\n", stats.allocated_blocks);
            printf("Free Blocks            : %d\n", stats.free_blocks);
            printf("Largest Free Block     : %zu KB\n", stats.largest_free_block);
            printf("Utilization            : %.2f%%\n", utilization);
            printf("External Fragmentation : %.2f%%\n",stats.external_fragmentation);
            printf("workload               : Show sample workload\n");
            printf("========================================\n");
        }

        else if (strcmp(token, "help") == 0)
        {
            printf("\n========== RAMLens Commands ==========\n");
            printf("alloc <process> <size>   Allocate memory\n");
            printf("free <process>           Free memory\n");
            printf("map                      Show memory map\n");
            printf("stats                    Show memory statistics\n");
            printf("strategy <name>          Change allocation strategy\n");
            printf("help                     Show commands\n");
            printf("exit                     Exit RAMLens\n");
            printf("======================================\n");
        }

        else if (strcmp(token, "strategy") == 0)
        {
            char *strategy_input = strtok(NULL, " ");

            if (strategy_input == NULL)
            {
                printf("Usage: strategy <first|best|worst|next>\n");
                continue;
            }

            if (strcmp(strategy_input, "first") == 0)
            {
                *strategy = FIRST_FIT;
            }
            else if (strcmp(strategy_input, "best") == 0)
            {
                *strategy = BEST_FIT;
            }
            else if (strcmp(strategy_input, "worst") == 0)
            {
                *strategy = WORST_FIT;
            }
            else if (strcmp(strategy_input, "next") == 0)
            {
                *strategy = NEXT_FIT;
            }
            else
            {
                printf("Unknown strategy: %s\n", strategy_input);
                continue;
            }

            printf("Strategy changed to %s\n",
                   strategy_name(*strategy));
        }

        else
        {
            printf("Unknown command: %s\n", token);
        }
    }
}