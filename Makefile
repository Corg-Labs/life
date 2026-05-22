CC     = gcc
CFLAGS = -O2 -Wall

life: life.c
	$(CC) $(CFLAGS) life.c -o life

run: life
	./life

clean:
	rm -f life

.PHONY: run clean
