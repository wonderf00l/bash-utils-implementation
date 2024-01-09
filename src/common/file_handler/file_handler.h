#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// file handler для удобной работы с файлом
typedef struct Handler {
  FILE *file;
  char filename[256];

  FILE *out_stream;
  FILE *err_stream;
} FileHandler;

int open_file(FileHandler *handler, char *filename, const char *mode);
int is_empty(FileHandler *Handler);
void output(FileHandler *handler);
int close_file(FileHandler *handler);
#endif