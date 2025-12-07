#include <stdio.h>
#include <assert.h>
#include <string.h>

void *malloc(size_t size);
void free(void *ptr);

void test_basic_allocation()
{
    printf("\n--- Test 1: Basic allocation ---\n");
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
    printf("\n--- Test 2: Memory Reuse (First Fit) ---\n");
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
    else if (ptrB < ptrA) 
    {
        printf("Success: Address reused (and likely merged with previous block)!\n");
    }
    else
    {
        printf("Failure: Allocater requested new space instead of reusing.\n");
        printf("Old: %p, New: %p\n", ptrA, ptrB);
        // Use assert to fail the test and stop testing.
        assert(0);
    }

    free(ptrB);
    printf("Passed: Memory reuse logic.\n\n");
}

void test_fragmentation_skip()
{
    printf("\n--- Test 3: Skipping too-small blocks ---\n");
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

void test_coalescing()
{
    printf("\n--- Test 4: Coalescing (Merging Blocks) ---\n");
    void *ptr1 = malloc(64);
    void *ptr2 = malloc(64);

    printf("Allocated ptr1 %p and ptr2 %p\n", ptr1, ptr2);

    free(ptr1);
    free(ptr2);

    void *big_block = malloc(100);

    printf("Requested 100 bytes. Got: %p\n", big_block);

    assert(big_block == ptr1);

    free(big_block);
    printf("Passed: Coalescing logic.\n\n");
}

void test_alignment()
{
    printf("\n--- Test 5: Alignment Check ---\n");

    void *p1 = malloc(1);
    void *p2 = malloc(5);
    void *p3 = malloc(11);

    printf("p1: %p\n", p1);
    printf("p2: %p\n", p2);
    printf("p3: %p\n", p3);

    assert(((size_t) p1 % 8) == 0 && ((size_t) p2 % 8) == 0);
    printf("Alignment check passed!\n\n");
    free(p1);
    free(p2);
    free(p3);
}

void test_calloc()
{
    printf("\n--- Test 6: Calloc (Zero Initialization) ---\n");
    int *arr = calloc(10, sizeof(int));

    assert(arr != NULL);
    int is_clean = 1;

    for (int i = 0; i < 10; i++)
    {
        if (arr[i] != 0)
        {
            is_clean = 0;
            printf("Failure: Index %d is not zero! Value: %d\n", i, arr[i]);
            break;
        }
    }

    assert(is_clean);
    free(arr);
    printf("Passed: Calloc logic.\n\n");
}

void test_realloc()
{
    printf("\n--- Test 7: Realloc (Expansion and Copy) ---\n");
    int *arr = malloc(2 * sizeof(int));
    arr[0] = 10;
    arr[1] = 20;
    printf("Original Array: [%d, %d] at %p\n", arr[0], arr[1], arr);

    // Reallocate to hold 4 integers
    int *new_arr = realloc(arr, 4 * sizeof(int));

    assert(new_arr != NULL);

    printf("New array location: %p\n", new_arr);

    // Verify old data exists
    assert(new_arr[0] == 10);
    assert(new_arr[1] == 20);

    // Verify we can use new space.
    new_arr[2] = 30;
    new_arr[3] = 40;
    assert(new_arr[2] == 30);
    assert(new_arr[3] == 40);
    printf("Full array: [%d, %d, %d, %d]\n", new_arr[0], new_arr[1], new_arr[2], new_arr[3]);

    free(new_arr);
    printf("Passed: Realloc logic.\n\n");
}

void test_splitting()
{
    printf("\n--- Test 8: Block splitting ---\n");
    void *big = malloc(512);
    printf("Allocated big block at %p\n", big);

    free(big);

    // If splitting works, this will take the first part of the 512 hole.
    void *small1 = malloc(128);
    printf("Allocated small block 1 at %p\n", small1);

    void *small2 = malloc(128);
    printf("Allocated small block 2 at %p\n", small2);

    if (small1 == big)
    {
        if (small2 > small1 && (char *) small2 < (char *) small1 + 512)
        {
            printf("Success: The second block fits inside the original big block\n");
        }
        else
        {
            printf("Failure: Secodnd block was allocated outside the split zone.\n");
        }
    }
    else
    {
        printf("Failure: First block didn't reuse the freed space.\n");
    }

    free(small1);
    free(small2);
    printf("Passed: Splitting logic\n\n");
}



int main(void)
{
    printf("Running custom malloc tests ...\n");

    test_basic_allocation();
    test_reuse();
    test_fragmentation_skip();
    test_coalescing();
    test_alignment();
    test_calloc();
    test_realloc();
    test_splitting();

    printf("All tests passed\n");

    return 0;
}