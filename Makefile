CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -D_XOPEN_SOURCE=500 -g3 -O0

LIBS = $(wildcard lib/*.c)
SOURCES = $(LIBS) main.c
OBJECTS = $(patsubst lib/%.c, obj/%.o, $(LIBS)) obj/main.o

run: build
	./a.out

build: a.out

a.out: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

obj/main.o: main.c
	$(CC) $(CFLAGS) $^ -c -o $@

obj/%.o: lib/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) $^ -c -o $@

.PHONY: build run
