CC = gcc
CFLAGS = -Wall -Wextra

all: lib test

lib: malloc.c
	$(CC) $(CFLAGS) -fPIC -shared -o voidstar.so malloc.c

test: test.c
	$(CC) $(CFLAGS) -o test_bin test.c

clean:
	rm -f voidstar.so test_bin