build: src/main.c
	cc src/main.c -Wall -o bin/main -g

run: bin/main
	./bin/main
