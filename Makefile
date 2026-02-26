build: src/main.c
	cc src/main.c -Wall -o bin/main -g

run: bin/main
	./bin/main

check: bin/main
	valgrind --leak-check=full --show-leak-kinds=all ./bin/main
