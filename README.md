# Voidstar: A Custom Memory Allocator in C

Voidstar is a production-style memory allocator built from scratch in C. It replaces the standard C library's `malloc`, `free`, `calloc`, and `realloc` functions, interacting directly with the Unix kernel via `sbrk`. 

This project demonstrates core systems programming concepts including manual memory management, pointer arithmetic, heap structure manipulation, and alignment strategies.

## Features

- **Custom Malloc:** Allocates memory using a linked-list metadata structure.
- **Memory Coalescing:** Automatically merges adjacent free blocks upon release to prevent external fragmentation.
- **Block Splitting:** efficient memory usage by slicing large free blocks into smaller chunks to satisfy specific requests (reducing internal fragmentation).
- **First-Fit Strategy:** Efficiently searches the heap for the first available block that satisfies a request.
- **8-Byte Alignment:** Ensures all returned pointers are aligned to 8-byte boundaries for CPU architecture compatibility.
- **Calloc Support:** Implements contiguous allocation with overflow protection and zero-initialization.
- **Realloc Support:** Handles dynamic resizing of memory blocks, including data preservation (memcpy) and relocation.
- **Debug Tools:** Includes a heap visualizer to inspect memory layout during runtime.

## Technical Implementation

### The Block Structure
Memory is managed using a singly linked list of metadata blocks embedded directly in the heap. Each block tracks its size, status, and neighbors.

```c
typedef struct block {
    size_t size;        // Size of the user data
    struct block *next; // Pointer to the next block
    int free;           // Status flag
    int magic;          // Corruption detection (Magic Number)
} Block;
```

### Key Algorithms
#### 1. Allocation (First-Fit with Splitting)
When `malloc(size)` is called:
1. **Alignment**: The request size is rounded up to the nearest multiple of 8.
2. **Search**: The list is traversed to find a free block where `block->size >= requested_size`.
3. **Split**: If the found block is significantly larger than requested, it is split into two: one for the user, and a smaller remainder block that stays free.
4. **Request**: If no block is found, `sbrk()` requests new heap space from the OS.

#### 2. Deallocation (Coalescing)
When `free(ptr)` is called:
1. **Validation**: The magic number is checked to prevent freeing invalid pointers (e.g., stack variables).
2. **Marking**: The block is flagged as free.
3. **Coalescing**: The list is scanned to merge adjacent free blocks (e.g., [Free A] -> [Free B] becomes [Big Free A]).

#### 3. Reallocation
When `realloc(ptr, size)` is called:
1. It checks if the current block is already large enough.
2. If not, it allocates a new block of size.
3. It copies data from the old block to the new one using memcpy.
4. It frees the old block.

### Uusage
To compile the project and run the test suite:
```Bash
make test
```
To use in your own code (requires linking):
```C
// Example usage
void *ptr = malloc(1024);
ptr = realloc(ptr, 2048);
free(ptr);
```
