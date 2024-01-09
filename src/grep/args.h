#ifndef ARGS_H
#define ARGS_H
#include <stdbool.h>
#include <stdio.h>

typedef struct opts {
  bool ignore_reg, search_inversion, rows_count, fnames_only, row_nums,
      no_fnames, suppress_err, truncated;
} Options;

typedef struct containers {
  char **templates;
  char **files_to_scan;
  size_t templates_size, fs_to_scan_sz;

  char *templ_filename;
} InitialData;

typedef struct data {
  char **matched_strings;
  size_t matched_str_size;
  char *filename;
  size_t row_num;
} RowData;

// rows_quantity
// provide -s option to file handler

typedef FILE *Stream;

#endif
