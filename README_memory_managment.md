// The memory management package should include a header file mem.h and C implementation files that specify and implement the following five functions.

// void* getmem(uintptr_t size). Return a pointer to a new block of storage with at least size bytes of memory. The pointer to the returned block should 
be aligned on an 16-byte boundary (i.e., its address should be a multiple of 16). The block may be somewhat larger than the size requested, if that is 
convenient for the memory allocation routines, but should not be significantly larger, which would waste space. The value size must be greater than 0. 
If size is not positive, or if for some reason getmem cannot satisfy the request, it should return NULL. Type uintptr_t is an unsigned integer type that 
can hold a pointer value (i.e., can be converted to or from a pointer of type void *, or other pointer type, exactly). It is defined in header <inttypes.h> 
(and also <stdint.h>). See discussion below.

// void freemem(void* p). Return the block of storage at location p to the pool of available free storage. The pointer value p must be one that was obtained 
as the result of a call to getmem. If p is NULL, then the call to freemem has no effect and returns immediately. If p has some value other than one returned by 
getmem, or if the block it points to has previously been released by another call to freemem, then the operation of freemem is undefined 
(i.e., freemem may behave in any manner it chooses, possibly causing the program to crash either immediately or later; it is under no obligation to detect or report such errors).
One additional implementation requirement: When freemem returns a block of storage to the pool, if the block is physically located in memory adjacent to one or more 
other free blocks, then the free blocks involved should be combined into a single larger block, rather than adding the small blocks to the free list individually.

// void get_mem_stats(uintptr_t* total_size, uintptr_t* total_free, uintptr_t* n_free_blocks). Store statistics about the current state of the memory manager in 
the three integer variables whose addresses are given as arguments. The information stored should be as follows:
total_size: total amount of storage in bytes acquired by the memory manager so far to use in satisfying allocation requests. (In other words, the total amount requested from the underlying system.)
total_free: the total amount of storage in bytes that is currently stored on the free list, including any space occupied by header information or links in the free blocks.
n_free_blocks: the total number of individual blocks currently stored on the free list.
See the discussion below outlining the implementation of the memory manager for more details about these quantities.

// void print_heap(FILE * f). Print a formatted listing on file f showing the blocks on the free list. Each line of output should describe one 
free block and begin with two hexadecimal numbers (0xdddddddd, where d is a hexadecimal digit) giving the address and length of that block. 
You may include any additional information you wish on the line describing the free block, but each free block should be described on a single output 
line that begins with the block's address and length.

// void check_heap(). Check for possible problems with the free list data structure. When called this function should use asserts to verify that the free list has the following properties:
Blocks are ordered with strictly increasing memory addresses Block sizes are positive numbers and no smaller than whatever minimum size you are using in your implementation
Blocks do not overlap (the start + length of a block is not an address in the middle of a later block on the list)
Blocks are not touching (the start + length of a block should not be the same address as the next block on the list since in that case the two blocks should have been combined into a single, larger block.)
If no errors are detected, this function should return silently after performing these tests. If an error is detected, then an assert should fail and cause the program to terminate at that point. Calls to check_heap 
should be included in other functions to attempt to catch errors as soon as possible. In particular, include calls to check_heap at the beginning and end of functions getmem and freemem. Include additional calls to check_heap wherever else it makes sense.
