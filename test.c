#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("Test program starting...\n");

    void *p = malloc(100);

    if (p == NULL)
    {
        printf("Got NULL! (This is expected for the skeleton)\n");
    }
    else
    {
        printf("Got memory\n");
        free(p);
    }

    return 0;
}