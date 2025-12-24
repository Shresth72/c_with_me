AGENTS=5

build:
	gcc -Wall -Wextra -g -o main main.c `sdl2-config --cflags --libs` -lm

run: build
	./main $(AGENTS) 
