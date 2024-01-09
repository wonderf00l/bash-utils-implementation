#include "file_handler.h"

int open_file(FileHandler *handler, char *filename,
              const char *mode) {  // need typedef struct for correct import
  int status = 0;
  if (!handler->file) {
    if (!filename) {
      handler->err_stream
          ? fprintf(handler->err_stream, "No file has been provided\n")
          : true;
      return -1;
    }
    strcpy(handler->filename, filename);
    handler->file = fopen(handler->filename, mode);
    if (!handler->file) {
      handler->err_stream
          ? fprintf(handler->err_stream,
                    "Problems while opening the file '%s'\n", filename)
          : true;
      status = -1;
    }
    return status;
  }
  handler->err_stream
      ? fprintf(handler->err_stream, "Another file ('%s') is opened\n",
                handler->filename)
      : true;
  status = -1;
  return status;
}

int is_empty(FileHandler *handler) {
  if (handler->file) {
    int sym = fgetc(handler->file);
    if (sym == EOF) {
      return 1;
    } else {
      ungetc(sym, handler->file);
      return 0;
    }
  } else {
    handler->err_stream
        ? fprintf(handler->err_stream,
                  "trying to check emptiness with NULL stream\n")
        : true;
    return -1;
  }
}

void output(FileHandler *handler) {
  if (handler->file) {
    char buf[1024];
    while (fgets(buf, sizeof(buf), handler->file)) {
      fprintf(handler->out_stream, "%s", buf);
    }
    fseek(handler->file, 0, SEEK_SET);
  }
}

int close_file(FileHandler *handler) {
  int status = -1;
  if (handler->file) {
    status = fclose(handler->file);
  } else {
    handler->err_stream ? fprintf(handler->err_stream, "no file was handled\n")
                        : true;
  }
  if (status == -1) {
    handler->err_stream
        ? fprintf(handler->err_stream, "problems while closing the file '%s'\n",
                  handler->filename)
        : true;
  } else {
    handler->file = NULL;
  }
  return status;
}
