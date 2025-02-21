CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -D_XOPEN_SOURCE=500 -g3 -O0

run: build
	./a.out

build: a.out

a.out: main.o lib/hd.o
	$(CC) $(CFLAGS) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) main.c -c -o main.o

lib/hd.o: lib/hd.c
	$(CC) $(CFLAGS) lib/hd.c -c -o lib/hd.o

.PHONY: build run
