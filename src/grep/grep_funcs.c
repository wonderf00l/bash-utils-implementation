#include <ctype.h>
#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "args.h"

extern char **append_str(char ***str_containter, size_t *size, char *str);
extern void free_container(char ***str_container, size_t *size);

// парсер аргументов командной строки
int parse_args(int argc, char *argv[], Options *opts, InitialData *init_data) {
  char opt;
  bool err = false;

  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (opt) {
      case 'i':
        opts->ignore_reg = true;
        break;
      case 'v':
        opts->search_inversion = true;
        break;
      case 'c':
        opts->rows_count = true;
        break;
      case 'l':
        opts->fnames_only = true;
        break;
      case 'n':
        opts->row_nums = true;
        break;
      case 'h':
        opts->no_fnames = true;
        break;
      case 's':
        opts->suppress_err = true;
        break;
      case 'o':
        opts->truncated = true;
        break;
      case 'e':
        append_str(&init_data->templates, &init_data->templates_size, optarg);
        break;
      case 'f':
        init_data->templ_filename = optarg;
        break;
      case '?':
        err = true;
        printf("UNEXPECTED OPTION\n");
    }
  }

  if (!init_data->templates &&
      !init_data->templ_filename) {  // grep -f file.txt -e templ.txt 'templ'
                                     // file.txt --> no file 'templ'
    append_str(&init_data->templates, &init_data->templates_size,
               argv[optind++]);
  }

  for (int i = optind; i < argc; ++i) {
    append_str(&init_data->files_to_scan, &init_data->fs_to_scan_sz, argv[i]);
  }

  if (!init_data->files_to_scan) {
    printf("NO FILES\n");
  }

  return (err || (!init_data->templates && !init_data->templ_filename) ||
          !init_data->files_to_scan)
             ? -1
             : 0;
}

// ищет вхождение строк/regex в строке
char *re_strstr(char *haystack, const char *needle, char **end, int opts,
                int *status) {
  char *start = NULL;
  *end = NULL;
  regex_t template;
  regmatch_t pmatch;
  if (!haystack) return NULL;
  if (regcomp(&template, needle, opts)) {
    fprintf(stderr, "Bad template: %s\n", needle);
    regfree(&template);
    *status = -1;
    return NULL;
  }
  if (!regexec(&template, haystack, 1, &pmatch, 0)) {
    start = (char *)haystack + pmatch.rm_so;
    *end = (char *)haystack + pmatch.rm_eo;
    *status = 0;
  }
  regfree(&template);
  return start;
}

int set_flags(Options *opts) {
  return REG_EXTENDED | (opts->ignore_reg ? REG_ICASE : 0);
}

// пишет в буфер end-start символов, начиная со start
char *make_str(char *dest, char *start, char *end) {
  if (!start || !end) {
    return NULL;
  }
  strncpy(dest, start, end - start);
  dest[end - start] = '\0';
  return dest;
}

// проверка того, все ли шаблоны были проверены на 1-ой конкретной строке
bool is_scrolled(int *arr, size_t size) {
  bool is_scrolled = true;

  for (size_t i = 0; i != size; ++i) {
    if (arr[i] != -1) {
      is_scrolled = false;
      break;
    }
  }

  return is_scrolled;
}

// извлечение частей строки, совпадающих с каждым из шаблонов контейнера
int extract_matched(char *string, char **templates, size_t t_size,
                    char ***res_container, size_t *c_size,
                    Options *opts) {  // extract matched

  int status = 0;

  int *offsets = (int *)calloc(t_size, sizeof(int));
  size_t curr = 0;

  while (!is_scrolled(offsets, t_size)) {
    if (offsets[curr] == -1) {
      curr = (curr + 1) % t_size;
      continue;
    }

    char *curr_pos = string + offsets[curr];
    char *end = NULL;
    curr_pos =
        re_strstr(curr_pos, templates[curr], &end, set_flags(opts), &status);
    if (status == -1) {
      break;
    }
    char buf[2048];

    if (curr_pos) {
      if (!opts->truncated) {
        append_str(res_container, c_size,
                   make_str(buf, string, string + strlen(string)));

        break;
      } else {
        append_str(res_container, c_size, make_str(buf, curr_pos, end));
      }
    }

    offsets[curr] = curr_pos ? end - string : -1;
    curr = (curr + 1) % t_size;
  }

  free(offsets);

  return status;
}

// сравнение подстрок по расстоянию от начала строки файла(флаг -o)
bool comparator(char *lhs, char *rhs, char *start) {
  return strstr(start, lhs) - start < strstr(start, rhs) - start;
}

void swap(char **lhs, char **rhs) {
  char *tmp = *lhs;
  *lhs = *rhs;
  *rhs = tmp;
}

// сортировка частей, удовлетворяющих шаблонам(флаг -o)
void sort(char **extracted_strings, size_t size, char *start) {
  for (size_t i = 0; i != size; ++i) {
    for (size_t j = 0; j != size - 1 - i; ++j) {
      if (comparator(extracted_strings[j + 1], extracted_strings[j], start)) {
        swap(extracted_strings + j, extracted_strings + j + 1);
      }
    }
  }
}

// принимает строку, извлекает нужные части строки, учитывая все поулченные
// опции(флаги)
int string_analyzer(char *string, char **templates, size_t t_size,
                    char ***matched_strings, size_t *c_size, Options *opts) {
  if (opts->truncated && opts->search_inversion) {
    return 0;
  }

  int status =
      extract_matched(string, templates, t_size, matched_strings, c_size, opts);

  if (status != -1) {
    if (opts->search_inversion) {
      if (*matched_strings) {
        free_container(matched_strings, c_size);
      } else {
        // printf("APPENDING: %s", string);
        append_str(matched_strings, c_size, string);
      }
    }

    sort(*matched_strings, *c_size, string);
  }

  return status;
}

// для извлечения шаблонов из файла
void extract_templates(FILE *templ_file, char ***templates, size_t *t_size) {
  char buf[2048];

  while (fgets(buf, sizeof(buf), templ_file)) {
    buf[strlen(buf) - 1] = '\0';
    append_str(templates, t_size, buf);
  }
}

// формирование префикса для вывода, пример: file.txt:15:abcae, 15:aweaew, awewa
char *make_prefix(char *prefix, Options *opts, InitialData *data,
                  RowData *row_data) {
  char file_prefix[256] = "";

  if (data->fs_to_scan_sz > 1 && !opts->no_fnames) {
    strcpy(file_prefix, row_data->filename);
    file_prefix[strlen(row_data->filename)] = ':';
    file_prefix[strlen(row_data->filename) + 1] = '\0';
  }
  strcat(prefix, file_prefix);

  char num_prefix[256];
  sprintf(num_prefix, "%ld", row_data->row_num);
  strcat(prefix, (opts->row_nums) ? strcat(num_prefix, ":") : "");

  return prefix;
}

// вывод строки с учетом сгенерированного префикса
void print_matched(FILE *stream, char *prefix, char **strings,
                   size_t strings_quantity) {
  for (size_t i = 0; i != strings_quantity; ++i) {
    fprintf(stream, "%s%s\n", prefix, strings[i]);
  }
}

// обертка для вывода строки
void row_printer(FILE *stream, Options *opts, InitialData *data,
                 RowData *row_data) {
  char prefix[256] = "";
  make_prefix(prefix, opts, data, row_data);
  print_matched(stream, prefix, row_data->matched_strings,
                row_data->matched_str_size);
}