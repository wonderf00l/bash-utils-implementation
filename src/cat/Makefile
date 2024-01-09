CC = gcc
CFLAGS = -Wall -Wextra -std=c11 #-fsanitize=address
# LDFLAGS = -fsanitize=address # Asan should be in a linking phase also
RM = rm -rf
TARGET_LIB = libhandler.a
LIB_PREFIX = ../common/file_handler/

all: s21_cat

s21_cat: s21_cat.o cat_funcs.o arg_parser.o $(TARGET_LIB)
	$(CC) s21_cat.o cat_funcs.o arg_parser.o -L. -lhandler -o $@

s21_cat.o: main.c
	$(CC) $(CFLAGS) -g -c $< -o $@

cat_funcs.o: cat_funcs.c
	$(CC) $(CFLAGS) -g -c $< -o $@

arg_parser.o: ../common/arg_parser/arg_parser.c
	$(CC) $(CFLAGS) -g -c $< -o $@


$(TARGET_LIB): handler.o
	ar r -o $(TARGET_LIB) handler.o

handler.o: $(LIB_PREFIX)file_handler.c $(LIB_PREFIX)file_handler.h
	$(CC) $(CFLAGS) -g -c $< -o $@

clean:
	$(RM) $(TARGET_LIB) *.o

rebuild:
	make clean
	make s21_cat

# $@ Имя цели обрабатываемого правила
# $< Имя первой зависимости обрабатываемого правила
# $^ Список всех зависимостей обрабатываемого правила