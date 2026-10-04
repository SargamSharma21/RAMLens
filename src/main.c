#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include "memory.h"
#include "allocator.h"
#include "cli.h"

int main(void) {
    Block *memory = memory_init(1024);

    if (memory == NULL)
    {
        printf("Failed to initialize memory.\n");
        return 1;
    }

    printf("RAMLens - Memory Management Simulator\n\n");

    AllocationStrategy strategy = FIRST_FIT;

    cli_run(&memory, &strategy);

    memory_destroy(memory);

    return 0;
}