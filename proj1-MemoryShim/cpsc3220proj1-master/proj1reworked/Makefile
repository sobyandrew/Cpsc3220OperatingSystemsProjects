CC=clang
CFLAGS=-Wall -g

BINS=leakcount memory_shim.so sctracer test


all: $(BINS)

leakcount: leakcount.c
	$(CC) $(CFLAGS) -o leakcount leakcount.c -Wall

memory_shim.so:  memory_shim.c
	$(CC) $(CFLAGS) -fPIC -shared -o memory_shim.so memory_shim.c -ldl -Wall

sctracer: sctracer.c
	$(CC) $(CFLAGS) -o sctracer sctracer.c -Wall

test: test.c
		$(CC) $(CFLAGS) -o test test.c -Wall

clean:
	rm $(BINS)
