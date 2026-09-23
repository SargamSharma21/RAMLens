#include <string.h>
#include <stdlib.h>

#include "allocator.h"

int memory_allocate(Block *head , const char *process_name , size_t size)
{
    Block *current = head;

    while(current != NULL) 
    {
        if(current->is_free && current->size >= size) 
        {
            if (current->size > size)
            {
                Block *new_block = malloc(sizeof(Block));

                if(new_block == NULL)
                {
                    return 0;
                }

                new_block->start = current->start + size;
                new_block->size = current->size - size;
                new_block->is_free = 1;
                new_block->process_name[0] = '\0';

                new_block->next = current->next;
                current->next = new_block;

                current->size = size;
            }

            current->is_free = 0;
            strcpy(current->process_name , process_name);

            return 1;
            
        }

        current = current->next;
    }

    return 0;
}

int memory_allocate_best_fit(
    Block *head , 
    const char *process_name,
    size_t size)
{
    Block *current = head;
    Block *best = NULL;

    while (current != NULL)
    {
        if (current->is_free && current->size >= size)
        {
            if (best == NULL || current->size < best -> size)
            {
                best = current;
            }
        }

        current = current->next;
    }

    if(best == NULL)
    {
        return 0;
    }

    if (best->size > size)
    {
        Block *new_block = malloc(sizeof(Block));

        if (new_block == NULL)
        {
            return 0;
        }

        new_block->start = best->start + size;
        new_block->size = best->size - size;
        new_block->is_free = 1;
        new_block->process_name[0] = '\0';

        new_block->next = best->next;
        best->next = new_block;

        best->size = size;
    }

    best->is_free = 0;
    strcpy(best->process_name, process_name);

    return 1;
}


int memory_free(Block *head , const char *process_name)
{
    Block *current = head;

    while (current != NULL)
    {
        if(!current->is_free &&
           strcmp(current->process_name , process_name) == 0)
        {
            current->is_free = 1;
            current->process_name[0] = '\0';

            return 1;
        }

        current = current->next;
    }

    return 0;
}

void memory_coalesce(Block *head)
{
    Block *current = head;

    while(current != NULL && current->next != NULL)
    {
        if(current->is_free && current->next->is_free)
        {
            Block *next = current->next;

            current->size += next->size;
            current->next = next->next;

            free(next);
        }
        else
        {
            current = current->next;
        }
    }
}