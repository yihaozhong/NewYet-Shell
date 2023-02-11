CC=gcc
CFLAGS=-g -pedantic -std=gnu17 -Wall -Werror -Wextra

.PHONY: all
all: newYetShell

newYetShell: newYetShell.o 

newYetShell.o: newYetShell.c

.PHONY: clean
clean:
	rm -f *.o newYetShell
