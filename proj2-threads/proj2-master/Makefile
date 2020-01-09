CC=clang
CFLAGS=-Wall -g

BINS=libmythreads.a

all: $(BINS)

libmythreads.a: mythreads.c
	$(CC) $(CFLAGS) -o libmythreads.a mythreads.c -Wall

clean:
	rm $(BINS)
