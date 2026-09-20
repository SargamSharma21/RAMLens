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

    memory_print(memory);

    printf("\nAllocating Chrome...\n");

    if(memory_allocate(memory , "Chrome" , 200)) 
    {
        printf("Allocation successful!\n");
    }
    else
    {
        printf("Allocation failed\n");
    }
    memory_print(memory);


    printf("\nFreeing Chrome...\n");

    if(memory_free(memory , "Chrome"))
    {
        printf("Deallocating successful!\n");
    }
    else 
    {
        printf("Deallocation failed!\n");
    }

    memory_print(memory);
    return 0;
}