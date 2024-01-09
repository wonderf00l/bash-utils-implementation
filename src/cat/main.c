#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../common/file_handler/file_handler.h"

#define OPT_SIZE 8

typedef struct Util {
  FileHandler handler;
  size_t abs_offset;
  char *options;
  size_t options_size;  // no map in c ({"squeeze":true}, {"number":true} ...)
                        // so will aggregate options in str like "bens"
} CatUtil;

extern char **parse_opts(int argc, char *raw_opts[], char *opt_pairs[][2],
                         size_t opt_pairs_size, char **parsed_opts,
                         size_t *parsed_opts_size, char ***not_dashed,
                         size_t *not_dashed_size);

extern void print_nonprinting(char ch);
extern void default_printing(char ch);

extern void number(FILE *file, size_t *abs_offset);
extern void number_nonblank(FILE *file, size_t *abs_offset);
extern void show_special(FILE *file, char special, const char *show_as,
                         void (*printer)(char));
extern void show_nonprinting(FILE *file);
extern void squeeze_blank(FILE *file);

extern void free_container(char ***str_container, size_t *size);

void start_util(CatUtil *util) {
  if (util->options[0] == '\0') {
    output(&util->handler);
    return;
  }

  switch (util->options[0]) {  // check only first option
    case 'n':
      number(util->handler.file, &util->abs_offset);
      break;
    case 'e':
      show_special(util->handler.file, '\n', "$\n", print_nonprinting);
      break;
    case 'E':
      show_special(util->handler.file, '\n', "$\n", default_printing);
      break;
    case 'b':
      number_nonblank(util->handler.file, &util->abs_offset);
      break;
    case 's':
      squeeze_blank(util->handler.file);
      break;
    case 't':
      show_special(util->handler.file, '\t', "^I", print_nonprinting);
      break;
    case 'T':
      show_special(util->handler.file, '\t', "^I", default_printing);
      break;
    case 'v':
      show_nonprinting(util->handler.file);
      break;
  }
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("Usage: [OPTION]... [FILE]...\n");
    exit(-1);
  }

  char *opt_pairs[OPT_SIZE][2] = {{"-b", "--number-nonblank"},
                                  {"-n", "--number"},
                                  {"-s", "--squeeze-blank"},
                                  {"-v", ""},
                                  {"-e", ""},
                                  {"-E", ""},
                                  {"-t", ""},
                                  {"-T", ""}};

  CatUtil util = {.handler = {.file = NULL,
                              .filename = {0},
                              .out_stream = stdout,
                              .err_stream = stderr},
                  .options = NULL,
                  .options_size = 0,
                  .abs_offset = 1};

  char **filenames_paths = NULL;
  size_t filenames_paths_size = 0;

  if (!(filenames_paths =
            parse_opts(argc, argv, opt_pairs, OPT_SIZE, &util.options,
                       &util.options_size, &filenames_paths,
                       &filenames_paths_size))) {  // realloc invalidates the
                                                   // pointer, so need rewrite
                                                   // or pointer on original ptr
    printf("Unsuccessful parsing, terminating\n");
    exit(-1);
  }

  for (size_t i = 0; i != filenames_paths_size; ++i) {
    if (open_file(&util.handler, filenames_paths[i], "r") == -1) {
      exit(-1);
    }
    start_util(&util);
    if (close_file(&util.handler) == -1) {
      exit(-1);
    }
  }

  free_container(&filenames_paths, &filenames_paths_size);
  free(util.options);
  return 0;
}