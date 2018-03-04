CC = gcc
CFLAGS = -Wall
OBJECT_FILES = bench.o freemem.o getmem.o get_mem_stats.o print_heap.o check_heap.o
SOURCE_FILES = bench.c freemem.c getmem.c get_mem_stats.c print_heap.c check_heap.c

bench: $(OBJECT_FILES)
	$(CC) $(CFLAGS) -o $@ $^

debug: $(SOURCE_FILES)
	$(CC) $(CFLAGS) -o benchdb $^
nodebug: $(SOURCE_FILES)
	$(CC) $(CFLAGS) -DNDEBUG -o benchndb $^

bench.o: bench.c mem.h
	$(CC) $(CFLAGS) -c $<

getmem.o: getmem.c mem_impl.h
	$(CC) $(CFLAGS) -c $<

freemem.o: freemem.c mem_impl.h
	$(CC) $(CFLAGS) -c $<

get_mem_stats.o: get_mem_stats.c mem_impl.h
	$(CC) $(CFLAGS) -c $<

print_heap.o: print_heap.c mem_impl.h
	$(CC) $(CFLAGS) -c $<

check_heap.o: check_heap.c mem_impl.h
	$(CC) $(CFLAGS) -c $<
	

depend: $(PROGRAM_C_FILES) 
	gcc -M $^ 

test:
	make bench; ./bench

dist:
	make clean; git >git.log log; tar -c *.c *.h Makefile git.log README.md


all: bench

clean:
	rm *.o *~ bench bench*db git.log


