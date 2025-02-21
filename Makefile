run: build
	./a.out

build:
	gcc -std=c11 -Wall -Wextra -g main.c

