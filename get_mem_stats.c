#include "mem_impl.h"
#include "mem.h"

// Counts the number of free blocks, totals the size of the free memory, and
// totals the size of memory requested from the underlying system.
// Needs pointers to the start of the free memory linked list (free_start) 
// and requested memory linkedlist (req_start) to be defined by the 
// calling program.

void get_mem_stats(uintptr_t* total_size, uintptr_t* total_free, uintptr_t* n_free_blocks) {
  extern BLOCK* free_start;
  extern uintptr_t total_mem;
  BLOCK* current;  // current block pointer in while loops
  uintptr_t f = 0; // will total up size of free memory
  uintptr_t n = 0; // will count free blocks
   
  current = (BLOCK*) free_start->next;
  while(current != NULL) {
    f = f + current->size;
    n ++;
    current = current->next;
  }
  
  *total_size = total_mem;
  *total_free = f;
  *n_free_blocks = n;
}

