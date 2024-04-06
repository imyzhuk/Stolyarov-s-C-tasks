CC = gcc
CFLAGS = -g -Wall

all:
	mkdir build
	$(CC) $(CFLAGS) -c modules/strings_utils.c -o build/strings_utils.o
	$(CC) $(CFLAGS) -c modules/stdin_handler.c -o build/stdin_handler.o
	$(CC) $(CFLAGS) src/5.15.c  build/stdin_handler.o build/strings_utils.o -o build/5.15

rebuild: clean all

run:
	./build/5.15

clean:
	rm -r build

stdin_handler.o: modules/stdin_handler.c modules/stdin_handler.h
	$(CC) $(CFLAGS) -c $< -o build/$@

strings_utils.o: modules/strings_utils.c modules/strings_utils.h
	$(CC) $(CFLAGS) -c $< -o build/$@
