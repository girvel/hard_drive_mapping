build:
	gcc -std=c11 -g main.c

run: build
	./a.out
