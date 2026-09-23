#include<stdio.h>
#include "memory.h"
#include "allocator.h"

int main(void) {
    Block *memory = memory_init(1024);

    if (memory == NULL)
    {
        printf("Failed to initialize memory.\n");
        return 1;
    }

    printf("RAMLens - Memory Management Simulator\n\n");

    printf("\nAllocating Chrome using First Fit...\n");

    if (memory_allocate(memory, "Chrome", 400))
    {
        printf("Allocation successful!\n");
    }
    else
    {
        printf("Allocation failed!\n");
    }

    memory_print(memory);

    printf("\nAllocating VSCode using First Fit...\n");

    if (memory_allocate(memory, "VSCode", 200))
    {
        printf("Allocation successful!\n");
    }
    else
    {
        printf("Allocation failed!\n");
    }

    memory_print(memory);

    printf("\nAllocating Google using First Fit...\n");

    if (memory_allocate(memory, "Google", 250))
    {
        printf("Allocation successful!\n");
    }
    else
    {
        printf("Allocation failed!\n");
    }

    memory_print(memory);

    printf("\nFreeing Chrome...\n");

    if (memory_free(memory, "Chrome"))
    {
        printf("Deallocation successful!\n");
        memory_coalesce(memory);
    }
    else
    {
        printf("Deallocation failed!\n");
    }

    printf("\nFreeing Google...\n");

    memory_print(memory);

    if (memory_free(memory, "Google"))
    {
        printf("Deallocation successeful!\n");
        // memory_coalesce(memory);
    }
    else
    {
        printf("Deallocation failed!\n");
    }

    memory_print(memory);

    printf("\nAllocating Firefox using Worst Fit...\n");

    if (memory_allocate_worst_fit(memory, "Firefox", 200))
    {
        printf("Worst-Fit allocation successful!\n");
    }
    else
    {
        printf("Worst-Fit allocation failed!\n");
    }

    memory_print(memory);

    return 0;
}