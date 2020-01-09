CC=clang
CFLAGS=-Wall -g

BINS=libmyalloc.so project3.tgz

libmyalloc.so:
	$(CC) $(CFLAGS) -fPIC -shared allocator.c -o libmyalloc.so

project3.tgz:
	tar -cvzf project3.tgz README Makefile allocator.c

clean:
	rm $(BINS)
