#include<stdio.h>
#include "memory.h"
int main(void) {
    Block *memory = memory_init(1024);

    if (memory == NULL)
    {
        printf("Failed to initialize memory.\n");
        return 1;
    }

    printf("RAMLens - Memory Management Simulator\n\n");

    printf("Start : %zu KB\n", memory->start);
    printf("Size  : %zu KB\n", memory->size);
    printf("Free  : %s\n", memory->is_free ? "Yes" : "No");

    memory_destroy(memory);

    return 0;
}