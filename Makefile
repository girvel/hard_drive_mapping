run: build
	./a.out

build:
	gcc -std=c11 -Wall -Wextra -D_XOPEN_SOURCE=500 -g3 -O0 main.c

