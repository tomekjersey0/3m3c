.SILENT all: build run

build:
	gcc -o bfs src/*.c -Iinclude

run:
	./bfs
