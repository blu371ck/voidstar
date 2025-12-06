#include <stdio.h>
#include <assert.h>
#include <string.h>

void *malloc(size_t size);
void free(void *ptr);

void test_basic_allocation()
{
    printf("--- Test 1: Basic allocation ---\n");
    int *ptr1 = malloc(sizeof(int));
    assert(ptr1 != NULL);
    *ptr1 = 42;
    printf("Allocated integer as %p with value %d\n", ptr1, *ptr1);

    // Testing not freeing it first for sequential allocation
    int *ptr2 = malloc(sizeof(int));
    assert(ptr2 != NULL);
    assert(ptr1 != ptr2); // Should be different addresses since its not freed yet.
    printf("Allocated second integer at %p\n", ptr2);

    free(ptr1);
    free(ptr2);
    printf("Passed: Basic allocation and free.\n\n");
}

void test_reuse()
{
    printf("--- Test 2: Memory Reuse (First Fit) ---\n");
    char *ptrA = malloc(128);
    assert(ptrA != NULL);
    printf("1. Allocated 128 bytes at %p\n", ptrA);

    // This should mark the block as free=1 in the linked list.
    free(ptrA);
    printf("2. Free memory at %p\n", ptrA);

    // Re-request the same size
    char *ptrB = malloc(128);
    assert(ptrB != NULL);
    printf("3. Allocated 128 bytes at %p\n", ptrB);

    if (ptrA == ptrB)
    {
        printf("Success: Memory address was reused! (ptrA == ptrB)\n");
    }
    else
    {
        printf("Failure: Allocater requested new space instead of reusing.\n");
        printf("Old: %p, New: %p\n", ptrA, ptrB);
        // Use assert to fail the test and stop testing.
        assert(ptrA == ptrB);
    }

    free(ptrB);
    printf("Passed: Memory reuse logic.\n\n");
}

void test_fragmentation_skip()
{
    printf("--- Test 3: Skipping too-small blocks ---\n");
    void *small = malloc(8);
    printf("Allocated small block at %p\n", small);

    void *large = malloc(4096);
    printf("Allocated large block at %p\n", large);

    free(small);
    printf("Freed small block\n");

    void *large2 = malloc(4096);
    printf("Allocated second large block at %p\n", large2);

    assert(large2 != small);
    printf("Success: Correctly skipped too-small free block\n");

    free(large);
    free(large2);
    printf("Passed: Fragmentation logic.\n\n");
}

int main(void)
{
    printf("Running custom malloc tests ...\n");

    test_basic_allocation();
    test_reuse();
    test_fragmentation_skip();

    printf("All tests passed\n");

    return 0;
}