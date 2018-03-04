#include "mem_impl.h"
#include "mem.h"

BLOCK* getmore(uintptr_t size);

BLOCK* free_start = NULL; // pointer to the top of the free block list
uintptr_t total_mem = 0; // counter of total malloc requests
  
void* getmem(uintptr_t size) {
  BLOCK* ptr; // this is what getmem returns.  It points to memory after header
  BLOCK* current; // these are used by the while look to step through the list
  BLOCK* previous;

  
  if(free_start == NULL) { //free_start is globally initialized to NULL,
    free_start = malloc(sizeof(BLOCK)); // this creates the actual block
    free_start->size = 0; // only once the first time getmem is called
    free_start->next = NULL; //points at the first block if freelist nonempty
  }
  if (size <= 0) return NULL; // only makes sense to request positive size
  size = size + sizeof(BLOCK); // stretch it to hold header block
  size = 16*(1+(size-1)/16); // round it up to the next multiple of 16
  check_heap(); // make sure the free list makes sense before starting
  
  previous = free_start;
  current = free_start->next; // start at the beginning of the free list
  while(current != NULL) { // last entry on list nas ->next == NULL
    if(current->size >= size) { //this block is big enough
      if(current->size >= size + MAX_WASTE) { //this block is too big
        current->size = current->size - size; // cut what we need from block
        ptr = (BLOCK*) (((uintptr_t)  current) + current->size); // our piece is after it
        ptr->size = size; // this is a fresh header, so fill in its data
        // ptr->next is filled in by freemem
      }
      else { // block is roughly the right size
        ptr = current; // take it all
        previous->next = current->next; // connect previous block to next
      }
      // in all cases where a big enough block is found,
      ptr->size = size; //record its size in the header
      check_heap(); // make sure free list still makes sense
      // ptr is of type BLOCK, so this skips past the block header
      return (void*) ++ptr; 
    } // this block was not big enough, so
    previous = previous->next; // keep looking
    current = current->next;
  }
  // no block was found that was big enough, so
  ptr = getmore(size);
  check_heap(); // make sure the free list still makes sense
  
  // ptr is of type BLOCK, so this skips past the block header
  return (void*) ++ptr; // points at the part of the block just after the header
}

// get another block from the system including some extra, split it,
// put the extra on the free list, and return a pointer to the block we want
BLOCK* getmore(uintptr_t size){
  BLOCK* newblock;
  
  newblock = (BLOCK*) malloc(size + EXTRA); // get some more memory from system
  newblock->size = EXTRA; // this is a fresh header, so fill in its data
  // after removing the block we want, EXTRA will be left so this is its size
  // newblock->next is filled in by freemem  
  freemem((void*) (newblock+1)); // use freemem to add the new block to the list
  // "newblock + 1" because freemem expects a pointer to just AFTER the header
  
  newblock = (BLOCK*) (((uintptr_t)newblock) + EXTRA); // cut our block from end
  newblock->size = size; // this is a fresh header, so fill in its data
  // newblock->next is filled in by freemem
  
  total_mem = total_mem + size + EXTRA; // track amount requested from system
  return newblock; // return pointer to header
}
