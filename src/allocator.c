#include <string.h>

#include "allocator.h"

int memory_allocate(Block *head , const char *process_name , size_t size)
{
    Block *current = head;

    while(current != NULL) 
    {
        if(current->is_free && current->size >= size) 
        {
            current->is_free = 0;
            strcpy(current->process_name , process_name);

            return 1;
        }

        current = current->next;
    }

    return 0;
}