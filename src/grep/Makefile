CC = gcc
CFLAGS = -Wall -Wextra -std=c11 #-fsanitize=address
RM = rm -rf
# LDFLAGS = -fsanitize=address
TARGET_LIB = libhandler.a
LIB_PREFIX = ../common/file_handler/

all: s21_grep

s21_grep: s21_grep.o arg_parser.o grep_funcs.o grep_core.o $(TARGET_LIB)
	$(CC) s21_grep.o arg_parser.o grep_funcs.o grep_core.o -L. -lhandler -o $@

s21_grep.o: main.c
	$(CC) $(CFLAGS) -g -c $< -o $@

arg_parser.o: ../common/arg_parser/arg_parser.c
	$(CC) $(CFLAGS) -g -c $< -o $@

grep_funcs.o: grep_funcs.c
	$(CC) $(CFLAGS) -g -c $< -o $@

grep_core.o: grep_core.c
	$(CC) $(CFLAGS) -g -c $< -o $@

$(TARGET_LIB): handler.o
	ar r -o $(TARGET_LIB) handler.o

handler.o: $(LIB_PREFIX)file_handler.c $(LIB_PREFIX)file_handler.h
	$(CC) $(CFLAGS) -g -c $< -o $@

clean:
	$(RM) $(TARGET_LIB) *.o

rebuild:
	make clean
	make s21_grep

# $@ Имя цели обрабатываемого правила
# $< Имя первой зависимости обрабатываемого правила
# $^ Список всех зависимостей обрабатываемого правила