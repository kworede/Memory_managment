#include "mem_impl.h"
#include "mem.h"
#include <assert.h>


void check_heap() {
  extern BLOCK* free_start;
  BLOCK* current;  // current block pointer in while loops
  int minblock = 16*(1+ (sizeof(BLOCK)+2)/16); //smallest block size should
  // be length of header + smallest mem request(=1), rounded up mod 16
  
  if(free_start->next == NULL) return; // list is empty, nothing to do
  current = (BLOCK*) free_start->next;
  
  // if list length < 2 do only the size test, others don't make sense
  if(current->next == NULL) assert(current->size >= minblock);
  
  while(current->next != NULL) {
    assert(current->size >= minblock);
    assert(current->next > current); // takes care of order
    // takes care of both touching and overlap:
    assert(current->size + (uintptr_t) current < (uintptr_t) current->next);
    current = current->next;
  }
return;
}

