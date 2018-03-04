#include "mem_impl.h"
#include "mem.h"

void insert_block(BLOCK* block, BLOCK* place);
void merge_blocks(BLOCK* block);

void freemem(void* p) {

  BLOCK* current;
  BLOCK* to_free;
  check_heap(); // make sure free list makes sense before we start
  to_free = (BLOCK*) p; // need this to access next, prev, and size
  to_free --; // move backward to the top of the block header
  
  current = free_start;
  while((current->next != NULL) && (current->next < to_free)) {
    current = current->next;
  }
  insert_block(to_free, current); // put 'to_free' into list after 'current'
  merge_blocks(to_free); // merge with next block
  merge_blocks(current); // merge with previous block
  check_heap();
}

// links 'block' into the free list immediately after 'place'
void insert_block(BLOCK* block, BLOCK* place) {

  block->next = place->next;
  place->next = block;
}

// detect touching blocks and merge them into a single bigger block
void merge_blocks(BLOCK* block) {

  if(block->size + (uintptr_t) block == (uintptr_t) block->next) {
    block->size = block->size + block->next->size; // compute merged size
    block->next = block->next->next; // link merged block to next block
  }
}
