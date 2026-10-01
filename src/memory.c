#include <stdio.h>
#include <stdlib.h>

#include "memory.h"

Block *memory_init(size_t size)
{
    Block *head = malloc(sizeof(Block));

    if (head == NULL)
    {
        return NULL;
    }

    head->start = 0;
    head->size = size;
    head->is_free = 1;
    head->process_name[0] = '\0';
    head->next = NULL;

    return head;
}

void memory_destroy(Block *head)
{
    while (head != NULL)
    {
        Block *next = head->next;

        free(head);

        head = next;
    }
}

void memory_print(Block *head)
{
    if (head == NULL)
    {
        printf("Memory is not initialized.\n");
        return;
    }

    printf("\n========== MEMORY MAP ==========\n");

    Block *current = head;

    while (current != NULL)
    {
        printf("Start: %zu KB | Size: %zu KB | ", 
               current->start, current->size);

        if (current->is_free)
        {
            printf("FREE\n");
        }
        else
        {
            printf("USED | Process: %s\n", current->process_name);
        }

        current = current->next;
    }

    printf("================================\n");
}


MemoryStats memory_get_stats(Block *head)
{
    MemoryStats stats = {0};
    Block *current = head;

    while(current != NULL) {
        if(current->is_free == 0) {
            stats.used_memory += current->size;
            stats.allocated_blocks++;
        }
        else {
            stats.free_memory += current->size;
            stats.free_blocks++;

            if (current->size > stats.largest_free_block)
            {
                stats.largest_free_block = current->size;
            }
        }


        current = current->next;
    }
    stats.total_memory = stats.used_memory + stats.free_memory;
    if (stats.free_memory > 0)
    {
        stats.external_fragmentation =
            (1.0 - ((double)stats.largest_free_block / stats.free_memory)) * 100.0;
    }
    else
    {
        stats.external_fragmentation = 0.0;
    }
    
    return stats;
}