# Voidstar: A Custom Memory Allocator in C

A functional implementation of `malloc`, `free`, and `calloc` built from scratch using Unix system calls. This project demonstrates core systems programming concepts including manual memory management, pointer arithmetic, and heap structure manipulation.

## Features

- **Custom Malloc:** Allocates memory using `sbrk` and a linked-list metadata structure.
- **Memory Coalescing:** Automatically merges adjacent free blocks upon release to prevent external fragmentation.
- **First-Fit Strategy:** Efficiently searches the heap for the first available block that satisfies a request.
- **Block Splitting:** (Optional: Add this if you did it, otherwise leave out)
- **8-Byte Alignment:** Ensures all returned pointers are aligned to 8-byte boundaries for CPU architecture compatibility.
- **Calloc Support:** Implements contiguous allocation with overflow protection and zero-initialization.
- **Debug Tools:** Includes a heap visualizer to inspect memory layout during runtime.

## Technical Implementation

### The Block Structure
Memory is managed using a linked list of metadata blocks embedded directly in the heap:
```c
typedef struct block {
    size_t size;        // Size of the user data
    struct block *next; // Pointer to the next block
    int free;           // Status flag
    int magic;          // Corruption detection
} Block;
```

### Allocation Strategy
1. **Search**: Traverses the linked list to find a free block (free == 1) with size >= requested_size.
2. **Request**: If no block is found, requests new heap space from the OS using sbrk().
3. **Return**: Returns a pointer to the user data payload, hiding the metadata header.

### Coalescing (De-fragmentation)
To solve the issue of "Swiss Cheese" memory (external fragmentation), the `free()` function scans the list. If it detects that a newly freed block is adjacent to another free block, it merges them into a single larger block in $O(1)$ relative to the scan.

### Usage
To compile the project and run the test suite:
```bash
make test
```
To use in your own code (requires linking):
```C
#include "malloc.h" // If you made a header file

void *ptr = malloc(1024);
free(ptr);
```

### Future Improvements
- Implement `realloc` for resizing blocks.
- Switch from a Singly Linked List to a Doubly Linked List for $O(1)$ block removal.
- Implement `memmove` logic for more efficient resizing.