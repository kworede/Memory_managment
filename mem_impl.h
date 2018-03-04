#ifndef MEM_IMPL
#define MEM_IMPL
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#define EXTRA 8192
#define MAX_WASTE 128

//linked list information for a memory block.
//every memory request will be increased by sizeof(block) to hold this data
typedef struct block {
  uintptr_t size;
  struct block* next;
} BLOCK;
  
extern BLOCK* free_start; // pointer to the top of the free block list
extern uintptr_t total_mem; // counter of total malloc requests
  
// inserts 'block' into the free list immediately after 'place'
void insert_block(BLOCK* block, BLOCK* place);

// detect touching blocks and merge them into a single bigger block
void merge_blocks(BLOCK* block); 
#endif
