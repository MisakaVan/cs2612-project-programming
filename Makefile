# macro for gcc
CC = gcc
ifdef V
	CFLAGS = -v
else
	CFLAGS =
endif

lexer.h: lang.l
	flex lang.l

lexer.c: lang.l
	flex lang.l

parser.c: lang.y
	bison -o parser.c -d -v lang.y

parser.h: lang.y
	bison -o parser.c -d -v lang.y

lang.o: lang.c lang.h
	$(CC) $(CFLAGS) -c lang.c

parser.o: parser.c parser.h lexer.h lang.h
	$(CC) $(CFLAGS) -c parser.c

lexer.o: lexer.c lexer.h parser.h lang.h
	$(CC) $(CFLAGS) -c lexer.c

lib.o: lib.c lib.h
	$(CC) $(CFLAGS) -c lib.c

main.o: main.c lexer.h parser.h lang.h lib.h
	$(CC) $(CFLAGS) -c main.c

main: lang.o parser.o lexer.o main.o lib.o
	$(CC) $(CFLAGS) lang.o parser.o lexer.o main.o lib.o -o main

all: main

clean:
	rm -f lexer.h lexer.c parser.h parser.c *.o main

%.c: %.y

%.c: %.l

.DEFAULT_GOAL := all
