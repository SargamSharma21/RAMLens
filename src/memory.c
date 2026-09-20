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