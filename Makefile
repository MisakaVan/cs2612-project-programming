# include batchtest.mk to use the test_all and clean_test_all targets.
include batchtest.mk

# macro for gcc
CC = gcc
ifdef V
	CFLAGS = -v
else
	CFLAGS =
endif

CFLAGS += -DASTPRINT

obj_dir = ./build
src_dir = ./src

LEXER_H = $(src_dir)/lexer.h
LEXER_C = $(src_dir)/lexer.c
PARSER_H = $(src_dir)/parser.h
PARSER_C = $(src_dir)/parser.c
LANG_H = $(src_dir)/lang.h
LANG_C = $(src_dir)/lang.c
LANG_L = $(src_dir)/lang.l
LANG_Y = $(src_dir)/lang.y
LIB_H = $(src_dir)/lib.h
LIB_C = $(src_dir)/lib.c
ASTPRINT_H = $(src_dir)/astprint.h
ASTPRINT_C = $(src_dir)/astprint.c
MAIN_C = $(src_dir)/main.c

PARSER_REPORT = parser_report.txt

OBJS = $(obj_dir)/lang.o $(obj_dir)/parser.o \
 $(obj_dir)/lexer.o $(obj_dir)/main.o $(obj_dir)/lib.o $(obj_dir)/astprint.o


build_dir:
	mkdir -p $(obj_dir)


$(LEXER_H): $(LANG_L)
	flex --outfile=$(LEXER_C) --header-file=$(LEXER_H) $(LANG_L)

$(LEXER_C): $(LANG_L)
	flex --outfile=$(LEXER_C) --header-file=$(LEXER_H) $(LANG_L)

$(PARSER_C): $(LANG_Y)
	bison -o $(PARSER_C) -d $(LANG_Y) --report=all --report-file=$(PARSER_REPORT)

$(PARSER_H): $(LANG_Y)
	bison -o $(PARSER_C) -d $(LANG_Y) --report=all --report-file=$(PARSER_REPORT)

$(obj_dir)/lang.o: $(LANG_C) $(LANG_H)
	$(CC) $(CFLAGS) -c $(LANG_C) -o $(obj_dir)/lang.o

$(obj_dir)/parser.o: $(PARSER_C) $(PARSER_H) $(LEXER_H) $(LANG_H)
	$(CC) $(CFLAGS) -c $(PARSER_C) -o $(obj_dir)/parser.o

$(obj_dir)/lexer.o: $(LEXER_C) $(LEXER_H) $(PARSER_H) $(LANG_H)
	$(CC) $(CFLAGS) -c $(LEXER_C) -o $(obj_dir)/lexer.o

$(obj_dir)/lib.o: $(LIB_C) $(LIB_H)
	$(CC) $(CFLAGS) -c $(LIB_C) -o $(obj_dir)/lib.o

$(obj_dir)/astprint.o: $(ASTPRINT_C) $(ASTPRINT_H)
	$(CC) $(CFLAGS) -c $(ASTPRINT_C) -o $(obj_dir)/astprint.o

$(obj_dir)/main.o: $(MAIN_C) $(LEXER_H) $(PARSER_H) $(LANG_H) $(LIB_H)
	$(CC) $(CFLAGS) -c $(MAIN_C) -o $(obj_dir)/main.o

main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o main

all: build_dir main

clean:
	rm -f $(LEXER_H) $(LEXER_C) $(PARSER_H) $(PARSER_C) $(obj_dir)/*.o main $(PARSER_REPORT)

%.c: %.y

%.c: %.l

.DEFAULT_GOAL := all
