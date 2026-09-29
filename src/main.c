#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include "memory.h"
#include "allocator.h"

int main(void) {
    Block *memory = memory_init(1024);
    AllocationStrategy strategy = FIRST_FIT;

    if (memory == NULL)
    {
        printf("Failed to initialize memory.\n");
        return 1;
    }

    printf("RAMLens - Memory Management Simulator\n\n");

    char command[100];

    while (1)
    {
        printf("RAMLens > ");

        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            break;
        }

        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "exit") == 0)
        {
            break;
        }

        char *token = strtok(command, " ");

        if (token == NULL)
        {
            continue;
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
                    strategy))
            {
                printf("Allocation successful!\n");
            }
            else
            {
                printf("Allocation failed!\n");
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
        else if (strcmp(token, "strategy") == 0)
        {
            char *strategy_name_input = strtok(NULL, " ");

            if (strategy_name_input == NULL)
            {
                printf("Usage: strategy <first|best|worst|next>\n");
                continue;
            }

            if (strcmp(strategy_name_input, "first") == 0)
            {
                strategy = FIRST_FIT;
            }
            else if (strcmp(strategy_name_input, "best") == 0)
            {
                strategy = BEST_FIT;
            }
            else if (strcmp(strategy_name_input, "worst") == 0)
            {
                strategy = WORST_FIT;
            }
            else if (strcmp(strategy_name_input, "next") == 0)
            {
                strategy = NEXT_FIT;
            }
            else
            {
                printf("Unknown strategy: %s\n", strategy_name_input);
                continue;
            }

            printf("Strategy changed to %s\n", strategy_name(strategy));
        }
        else if (strcmp(token, "map") == 0)
        {
            memory_print(memory);
        }
        else if (strcmp(token, "exit") == 0)
        {
            break;
        }
        else
        {
            printf("Unknown command: %s\n", token);
        }
    }
    
    return 0;
}