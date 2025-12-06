# voidstar

**voidstar** is a custom implementation of the C memory allocation functions (`malloc`, `free`, `realloc`) created to understand the low-level mechanics of memory management and system calls.

## Features (In Progress)
- [ ] Memory allocation using `sbrk`
- [ ] Header-based metadata for block management
- [ ] Pointer arithmetic for traversing the heap
- [ ] Free list management (reusing freed blocks)

## usage

To use this allocator, compile the shared library and use `LD_PRELOAD` to inject it into any existing process.

### Building
```bash
make
```

### Running with a test program
```bash
# Force the system to use voidstar.so instead of standard malloc
LD_PRELOAD=./voidstar.so ./test_bin
```
