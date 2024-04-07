CC = gcc
CFLAGS = -g -Wall
FILE_NAME = 5.19

all:
	mkdir build
	$(CC) $(CFLAGS) -c modules/strings_utils.c -o build/strings_utils.o
	$(CC) $(CFLAGS) -c modules/stdin_handler.c -o build/stdin_handler.o
	$(CC) $(CFLAGS) src/$(FILE_NAME).c  build/stdin_handler.o build/strings_utils.o -o build/$(FILE_NAME)

rebuild: clean all

run:
	./build/$(FILE_NAME)

clean:
	rm -r build

$(FILE_NAME):
	$(CC) $(CFLAGS) src/$(FILE_NAME).c  build/stdin_handler.o build/strings_utils.o -o build/$(FILE_NAME)
	
stdin_handler.o: modules/stdin_handler.c modules/stdin_handler.h
	$(CC) $(CFLAGS) -c $< -o build/$@

strings_utils.o: modules/strings_utils.c modules/strings_utils.h
	$(CC) $(CFLAGS) -c $< -o build/$@
