#include <string.h>
#include <stdlib.h>

#include "allocator.h"

static Block *next_fit_position = NULL;

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

int memory_allocate_worst_fit(
    Block *head , 
    const char *process_name,
    size_t size)
{
    Block *current = head;
    Block *worst = NULL;

    while(current != NULL) {
        if(current->is_free && current->size >= size) {
            if(worst == NULL || current->size > worst->size) {
                worst = current;
            }
        }

        current = current -> next;

    }

    if(worst == NULL) {
        return 0;
    }

    if(worst->size > size) {
        Block *new_block = malloc(sizeof(Block));

        if(new_block == NULL) {
            return 0;
        }
        
        new_block->start = worst->start + size;
        new_block->size = worst->size - size;
        new_block->is_free = 1;
        new_block->process_name[0] = '\0';

        new_block->next = worst->next;
        worst->next = new_block;

        worst->size = size;
    }

    worst->is_free = 0;
    strcpy(worst->process_name , process_name);

    return 1;
}

int memory_allocate_next_fit(
    Block *head,
    const char *process_name,
    size_t size)
{
    if (head == NULL)
    {
        return 0;
    }

    if (next_fit_position == NULL)
    {
        next_fit_position = head;
    }

    Block *current = next_fit_position;
    Block *start = current;

    while (1)
    {
        if (current->is_free && current->size >= size)
        {
            // found a suitable block
            break;
        }

        if (current->next != NULL)
        {
            current = current->next;
        }
        else
        {
            current = head;
        }

        if (current == start)
        {
            return 0;   // checked every block
        }
    }

    if (current->size > size)
    {
        Block *new_block = malloc(sizeof(Block));

        if (new_block == NULL)
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
    strcpy(current->process_name, process_name);
    if (current->next != NULL)
    {
        next_fit_position = current->next;
    }
    else
    {
        next_fit_position = head;
    }

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