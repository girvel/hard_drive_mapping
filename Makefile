run: build
	./a.out

build:
	gcc -std=c11 -Wall -Wextra -g3 -O0 main.c

