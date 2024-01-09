#include <stdio.h>
#include <stdlib.h>

#include "../common/file_handler/file_handler.h"
#include "args.h"

extern int parse_args(int argc, char *argv[], Options *opts, InitialData *data);
extern void extract_templates(FILE *templ_file, char ***templates,
                              size_t *t_size);
extern int scan_file(FILE *file, char *filename, InitialData *data,
                     Options *opts);

extern void free_container(char ***str_container, size_t *size);

int main(int argc, char *argv[]) {
  if (argc == 1) {
    fprintf(stdout, "Usage: [OPTION]... PATTERNS [FILE]...\n");
    exit(-1);
  }

  Options options = {0};
  InitialData init_data = {0};

  if (parse_args(argc, argv, &options, &init_data) == -1) {
    fprintf(stderr, "Problems while parsing, terminating\n");
    exit(-1);
  }

  FileHandler handler = {.file = NULL,
                         .filename = {0},
                         .out_stream = stdout,
                         .err_stream = options.suppress_err ? NULL : stderr};

  if (init_data.templ_filename &&
      open_file(&handler, init_data.templ_filename, "r") != -1) {
    extract_templates(handler.file, &init_data.templates,
                      &init_data.templates_size);
    close_file(&handler);
  }

  for (size_t i = 0; i != init_data.fs_to_scan_sz; ++i) {
    int status = 0;

    if (open_file(&handler, init_data.files_to_scan[i], "r") != -1) {
      status = scan_file(handler.file, handler.filename, &init_data, &options);
      close_file(&handler);
    }

    if (status == -1) {
      break;
    }
  }

  free_container(&init_data.templates, &init_data.templates_size);
  free_container(&init_data.files_to_scan, &init_data.fs_to_scan_sz);
}