CC=gcc
CFLAGS=-I -ansi -lm
DEBUGFLAGS=-Wall -Werror
DEPS=main.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

debug: main.o
	$(CC) -o main main.o $(DEBUGFLAGS)
	rm -rf main.o
	
debugRun: main.o
	$(CC) -o main main.o $(DEBUGFLAGS)
	rm -rf main.o
	./main

release: main.o
	$(CC) -o main main.o
	rm -rf main.o

clean:
	rm -rf main
	rm -rf main.o
