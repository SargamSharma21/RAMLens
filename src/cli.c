#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#include "workload.h"
#include "cli.h"

void cli_run(Block **memory, AllocationStrategy *strategy)
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

        /* ================= ALLOCATE ================= */

        if (strcmp(token, "alloc") == 0)
        {
            char *process_name = strtok(NULL, " ");
            char *size_string = strtok(NULL, " ");

            if (process_name == NULL || size_string == NULL)
            {
                printf("Usage: alloc <process> <size>\n");
                continue;
            }

            /*
             * Reject negative memory sizes before
             * calling strtoul().
             */
            if (size_string[0] == '-')
            {
                printf("Memory size cannot be negative.\n");
                continue;
            }

            char *endptr;

            errno = 0;

            unsigned long value =
                strtoul(size_string, &endptr, 10);

            /*
             * No digits were found or extra characters
             * were present after the number.
             */
            if (endptr == size_string || *endptr != '\0')
            {
                printf("Invalid memory size: %s\n", size_string);
                continue;
            }

            /*
             * Check for overflow.
             */
            if (errno == ERANGE || value > SIZE_MAX)
            {
                printf("Memory size is too large.\n");
                continue;
            }

            /*
             * Zero-sized allocations are not allowed.
             */
            if (value == 0)
            {
                printf("Memory size must be greater than 0.\n");
                continue;
            }

            size_t size = (size_t)value;

            if (memory_allocate_with_strategy(
                    *memory,
                    process_name,
                    size,
                    *strategy))
            {
                printf("Allocation successful!\n");
            }
            else
            {
                printf("Allocation failed!\n");

                MemoryStats stats =
                    memory_get_stats(*memory);

                printf("\n========== ALLOCATION FAILURE ==========\n");
                printf("Requested Memory   : %zu KB\n", size);
                printf("Total Free Memory  : %zu KB\n",
                       stats.free_memory);
                printf("Largest Free Block : %zu KB\n",
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

            memory_print(*memory);
        }

        /* ================= FREE ================= */

        else if (strcmp(token, "free") == 0)
        {
            char *process_name = strtok(NULL, " ");

            if (process_name == NULL)
            {
                printf("Usage: free <process>\n");
                continue;
            }

            if (memory_free(*memory, process_name))
            {
                printf("Deallocation successful!\n");
                memory_coalesce(*memory);
            }
            else
            {
                printf("Deallocation failed!\n");
            }

            memory_print(*memory);
        }

        /* ================= BENCHMARK ================= */

        else if (strcmp(token, "benchmark") == 0)
        {
            char *workload_input = strtok(NULL, " ");

            if (workload_input == NULL)
            {
                printf("Usage: benchmark <basic|fragmentation|stress>\n");
                continue;
            }

            WorkloadType workload_type;

            if (strcmp(workload_input, "basic") == 0)
            {
                workload_type = WORKLOAD_BASIC;
            }
            else if (strcmp(workload_input, "fragmentation") == 0)
            {
                workload_type = WORKLOAD_FRAGMENTATION;
            }
            else if (strcmp(workload_input, "stress") == 0)
            {
                workload_type = WORKLOAD_STRESS;
            }
            else
            {
                printf("Unknown workload: %s\n", workload_input);
                printf("Available workloads: basic, fragmentation, stress\n");
                continue;
            }

            benchmark_run(workload_type);
        }

        /* ================= WORKLOAD ================= */

        else if (strcmp(token, "workload") == 0)
        {
            char *workload_input = strtok(NULL, " ");

            if (workload_input == NULL)
            {
                printf("Usage: workload <basic|fragmentation|stress>\n");
                continue;
            }

            WorkloadType workload_type;

            if (strcmp(workload_input, "basic") == 0)
            {
                workload_type = WORKLOAD_BASIC;
            }
            else if (strcmp(workload_input, "fragmentation") == 0)
            {
                workload_type = WORKLOAD_FRAGMENTATION;
            }
            else if (strcmp(workload_input, "stress") == 0)
            {
                workload_type = WORKLOAD_STRESS;
            }
            else
            {
                printf("Unknown workload: %s\n", workload_input);
                printf("Available workloads: basic, fragmentation, stress\n");
                continue;
            }

            workload_run(
                *memory,
                *strategy,
                workload_type,
                1
            );
        }

        /* ================= MAP ================= */

        else if (strcmp(token, "map") == 0)
        {
            memory_print(*memory);
        }

        /* ================= STATS ================= */

        else if (strcmp(token, "stats") == 0)
        {
            MemoryStats stats =
                memory_get_stats(*memory);

            double utilization = 0.0;

            if (stats.total_memory > 0)
            {
                utilization =
                    ((double)stats.used_memory /
                     stats.total_memory) * 100.0;
            }

            printf("\n========== MEMORY STATISTICS ==========\n");
            printf("Total Memory           : %zu KB\n",
                   stats.total_memory);
            printf("Used Memory            : %zu KB\n",
                   stats.used_memory);
            printf("Free Memory            : %zu KB\n",
                   stats.free_memory);
            printf("Allocated Blocks       : %d\n",
                   stats.allocated_blocks);
            printf("Free Blocks            : %d\n",
                   stats.free_blocks);
            printf("Largest Free Block     : %zu KB\n",
                   stats.largest_free_block);
            printf("Utilization            : %.2f%%\n",
                   utilization);
            printf("External Fragmentation : %.2f%%\n",
                   stats.external_fragmentation);
            printf("========================================\n");
        }

        /* ================= HELP ================= */

        else if (strcmp(token, "help") == 0)
        {
            printf("\n========== RAMLens Commands ==========\n");
            printf("alloc <process> <size>   Allocate memory\n");
            printf("free <process>           Free memory\n");
            printf("map                      Show memory map\n");
            printf("stats                    Show memory statistics\n");
            printf("strategy <name>          Change allocation strategy\n");
            printf("help                     Show commands\n");
            printf("workload <type>          Run a workload\n");
            printf("                         Types: basic, fragmentation, stress\n");
            printf("benchmark <type>         Benchmark a workload\n");
            printf("                         Types: basic, fragmentation, stress\n");
            printf("exit                     Exit RAMLens\n");
            printf("reset                    Reset memory to initial state\n");
            printf("======================================\n");
        }

        /* ================= STRATEGY ================= */

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

        /* ================= RESET ================= */

        else if (strcmp(token, "reset") == 0)
        {
            memory_destroy(*memory);

            *memory = memory_init(1024);

            if (*memory == NULL)
            {
                printf("Memory reset failed!\n");
                return;
            }

            printf("Memory reset successfully!\n");
            memory_print(*memory);
        }

        /* ================= UNKNOWN COMMAND ================= */

        else
        {
            printf("Unknown command: %s\n", token);
        }
    }
}