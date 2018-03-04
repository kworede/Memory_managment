#include "mem_impl.h"
#include "mem.h"

// Prints a listing of all free memory block locations and sizes to a file.
// Needs pointers to the start of the free memory linked list (free_start) 
// and requested memory linkedlist (req_start) to be defined by the 
// calling program.

void print_heap(FILE* f) {
  BLOCK* current;
  
  current = (BLOCK*) free_start->next;
  fprintf(f, "start: %" PRIuPTR "\n", (uintptr_t) free_start);
  while(current != NULL) {
    fprintf(f, "Location: %" PRIuPTR "\t", (uintptr_t) current);
    fprintf(f, "Size: %i \t", current->size);
    fprintf(f, "Next: %" PRIuPTR "\n", (uintptr_t) current->next);
    current = current->next;
  }
}

