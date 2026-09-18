#include<stdio.h>
#include "memory.h"
int main(void) {
    Block block;

    block.start = 0;
    block.size = 200;
    block.is_free = 0;

    printf("RAMLens - Memory Management Simulator\n");
    printf("Block start : %zu KB\n", block.start);
    printf("Block size  : %zu KB\n", block.size);


    return 0;
}