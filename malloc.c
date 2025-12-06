#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct block
{
    size_t size;
    struct block *next;
    int free;
    int magic;
} Block;

void *global_base = NULL;

#define META_SIZE sizeof(struct block)

Block *request_space(Block *last, size_t size)
{
    Block *block;
    block = sbrk(0);

    void *request = sbrk(size + META_SIZE);
    
    if (request == (void*) -1)
    {
        return NULL;
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

void log_message(char *message)
{
    size_t len = 0;

    while (message[len] != '\0')
    {
        len++;
    }

    write(1, message, len);
    write(1, "\n", 1);
}

void *malloc(size_t size)
{
    log_message("[VOIDSTAR] Allocating memory...");

    Block *block;

    if (size <= 0)
    {
        return NULL;
    }

    if (global_base == NULL)
    {
        block = request_space(NULL, size);

        if (!block)
        {
            return NULL;
        }

        global_base = block;

        return (void *) (block + 1);
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

        return (void *) (ptr + 1);
    }

    return NULL;
}

void free(void *ptr)
{
    log_message("[VOIDSTAR] Freeing memory...");
}

