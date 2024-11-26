# macro for gcc
CC = gcc
ifdef V
	CFLAGS = -v
else
	CFLAGS =
endif

obj_dir = ./build

build_dir:
	mkdir -p $(obj_dir)

lexer.h: lang.l
	flex lang.l

lexer.c: lang.l
	flex lang.l

parser.c: lang.y
	bison -o parser.c -d lang.y --report=all

parser.h: lang.y
	bison -o parser.c -d lang.y --report=all

$(obj_dir)/lang.o: lang.c lang.h
	$(CC) $(CFLAGS) -c lang.c -o $(obj_dir)/lang.o

$(obj_dir)/parser.o: parser.c parser.h lexer.h lang.h
	$(CC) $(CFLAGS) -c parser.c -o $(obj_dir)/parser.o

$(obj_dir)/lexer.o: lexer.c lexer.h parser.h lang.h
	$(CC) $(CFLAGS) -c lexer.c -o $(obj_dir)/lexer.o

$(obj_dir)/lib.o: lib.c lib.h
	$(CC) $(CFLAGS) -c lib.c -o $(obj_dir)/lib.o

$(obj_dir)/main.o: main.c lexer.h parser.h lang.h lib.h
	$(CC) $(CFLAGS) -c main.c -o $(obj_dir)/main.o

main: $(obj_dir)/lang.o $(obj_dir)/parser.o $(obj_dir)/lexer.o $(obj_dir)/main.o $(obj_dir)/lib.o
	$(CC) $(CFLAGS) $(obj_dir)/lang.o $(obj_dir)/parser.o $(obj_dir)/lexer.o $(obj_dir)/main.o $(obj_dir)/lib.o -o main

all: build_dir main

clean:
	rm -f lexer.h lexer.c parser.h parser.c $(obj_dir)/*.o main

%.c: %.y

%.c: %.l

.DEFAULT_GOAL := all
