cc=clang
CFLAGS=-Wall -g

BINS=notjustcats

notjustcats:
	$(CC) -o notjustcats notjustcats.c

project4.tgz:
		tar -czvf project4.tgz README Makefile notjustcats.c

out:
	rm out*
	
clean:
	rm $(BINS)
