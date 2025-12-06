#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h> // Only used for standard definitions, not for the real malloc.
#include <errno.h> // Good for handling sbrk errors

/* --- Data Structures --- */

/**
 * Block Metadata
 * * This structure is placed immediately before the memory returned to the user.
 * It tracks the size of the allocation and maintains the linked list.
 */
typedef struct block
{
    size_t size;            // Size of the data part (excluding metadata)
    struct block *next;     // Pointer to the next block in the list
    int free;               // Flag: 1 is free, 0 is allocated
    int magic;              // Debugging magic number to detect coruption
} Block;

#define META_SIZE sizeof(struct block)
#define MAGIC_NUMBER 0x12345678

/* --- Global Variables --- */

// Specific to this file (static), preventing conflicts with user code.
static void *global_base = NULL;


/* --- Helper Functions --- */

/**
 * log_message
 * * Writes a message to stdout using the write() syscall.
 * * NOTE: We use write() instead of printf() because printf() internally
 * calls malloc(). Calling printf inside our own malloc would cause 
 * infinite recursion and crash the program.
 * * @param message The message to be printed.
 */
static void log_message(char *message)
{
    size_t len = 0;

    while (message[len] != '\0')
    {
        len++;
    }

    write(STDOUT_FILENO, message, len);
    write(STDOUT_FILENO, "\n", 1);
}

/**
 * request_space
 * * Asks the OS for more heap memory using sbrk().
 * Creates a new block and appends it to the linked list if "last" is provided.
 * * @param last The current last block in the linked list (can be NULL).
 * @param size The size of the memory requested (excluding metadata).
 * @return Pointer to the new Block, or NULL on failure.
 */
static Block *request_space(Block *last, size_t size)
{
    // sbrk(0) gives current break, but srbk(size) returns the *previous* break.
    // So, we can just call srbk(size) once to get the pointer.
    Block *block = sbrk(0);
    void *request = sbrk(size + META_SIZE);
    
    if (request == (void*) -1)
    {
        return NULL; // srbk failed (potential out of memory).
    }

    if (last)
    {
        last->next = block;
    }

    block->size = size;
    block->next = NULL;
    block->free = 0;
    block->magic = 0x12345678; // Used for debugging.

    return block;
}

/**
 * find_last_block
 * * Traverses the linked list to find the last node.
 * * @param base The head of the linked list.
 * @return The last block in the list.
 */
static Block *find_last_block(Block *base)
{
    Block *ptr = base;

    while (ptr && ptr->next != NULL)
    {
        ptr = ptr->next;
    }

    return ptr;
}

/* --- Public API --- */

/**
 * malloc
 * * Allocates size bytes of memory and returns a pointer to the allocated memory.
 * * @param size Number of bytes to allocate.
 * @return Pointer to allocated memory, or NULL if request fails.
 */
void *malloc(size_t size)
{
    log_message("[VOIDSTAR] Allocating memory...");

    Block *block;

    if (size <= 0)
    {
        return NULL;
    }

    // First allocation: Initialize the heap.
    if (global_base == NULL)
    {
        block = request_space(NULL, size);

        if (!block)
        {
            return NULL;
        }

        global_base = block;
    }
    else
    {
        Block *ptr = global_base;

        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }

        ptr = request_space(ptr, size);

        if (!ptr)
        {
            return NULL;
        }
    }

    // Return a pointer to the memory *after* the metadata block
    // (block + 1) increments the pointer by sizeof(Block) bytes.
    return (void *) (block + 1);
}

/**
 * free
 * * Frees the memory space pointed to by ptr.
 * * @param ptr Pointer to occupied memory to free.
 */
void free(void *ptr)
{
    if (!ptr)
    {
        return;
    }

    Block *block_ptr = (Block *) ptr - 1;

    if (block_ptr->magic != MAGIC_NUMBER)
    {
        log_message("[VOIDSTAR] Error: Invalid pointer passed to free!");
        return;
    }

    block_ptr->free = 1;

    log_message("[VOIDSTAR] Freeing memory...");
}

