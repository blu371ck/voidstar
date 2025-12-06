#include <unistd.h>
#include <stddef.h>

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

    return NULL;
}

void free(void *ptr)
{
    log_message("[VOIDSTAR] Freeing memory...");
}