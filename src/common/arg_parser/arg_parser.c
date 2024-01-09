#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strchr_(const char *s, size_t s_size, int sym) {
  if (!s) {
    return NULL;
  }

  size_t size = 0;
  while (size++ != s_size - 1 && *s != sym) {
    ++s;
  }

  return *s == sym ? (char *)s : NULL;
}

char *append_char(
    char **str, size_t *size,
    char sym) {  // либо передавать char** str и делать realloc исходного
                 // указателя, а не копии, либо char* str и всегда присваивать
                 // return value указателю, по которому добавляем значение, но
                 // новое значение нужно присваивать указателям из исходного
                 // внешнего скоупа, не копии в функции
  *str = (char *)realloc(*str, ++(*size) * sizeof(char));
  if (!*str) {
    printf("Bad alloc while appending char\n");
    return NULL;
  }
  (*str)[*size - 1] = sym;
  return *str;
}

char **append_str(char ***str_containter, size_t *size, char *str) {
  if (!str) {
    return NULL;
  }

  *str_containter =
      (char **)realloc(*str_containter, ++(*size) * sizeof(char *));
  if (!*str_containter) {
    printf("Bad alloc while appending str\n");
    return NULL;
  }
  (*str_containter)[*size - 1] = (char *)calloc(1, strlen(str) + 1);  // \0 also
  if (!(*str_containter)[*size - 1]) {
    printf("Bad alloc while appending str\n");
    return NULL;
  }

  strcpy((*str_containter)[*size - 1], str);
  return *str_containter;
}

void free_container(char ***str_container, size_t *size) {
  if (*str_container && size) {
    for (size_t i = 0; i != *size; ++i) {
      free((*str_container)[i]);
    }
    free(*str_container);
    *size = 0;
    *str_container = NULL;
  }
}

char *check_option(char *opt_pairs[][2], size_t opt_pairs_size,
                   const char *option) {
  char *short_opt = NULL;

  for (size_t i = 0; i != opt_pairs_size; ++i) {
    if (!strcmp(option, opt_pairs[i][0]) || !strcmp(option, opt_pairs[i][1])) {
      short_opt = opt_pairs[i][0];
    }
  }
  return short_opt;
}

// парсер для обработки пар опций(short, GNU) и остальных аргументов, валидации
// и агрегации результата в строки options и контейнер др аргументов(имена
// файлов и т.п.)
char **parse_opts(int argc, char *raw_opts[], char *opt_pairs[][2],
                  size_t opt_pairs_size, char **parsed_opts,
                  size_t *parsed_opts_size, char ***not_dashed,
                  size_t *not_dashed_size) {
  int opt_id;

  for (opt_id = 1; opt_id < argc; ++opt_id) {
    if (raw_opts[opt_id][0] != '-') {
      if (!append_str(not_dashed, not_dashed_size, raw_opts[opt_id])) {
        printf("BAD PARSING: memory allocation problem\n");
        break;
      }
      continue;
    }

    char *short_option;  // "-h", "-e", "-E", ...

    if ((short_option =
             check_option(opt_pairs, opt_pairs_size, raw_opts[opt_id]))) {
      char truncated_opt = short_option[1];  // 'h', 'e', ...
      if (!strchr_(*parsed_opts, *parsed_opts_size, truncated_opt)) {
        if (!append_char(parsed_opts, parsed_opts_size, truncated_opt)) {
          printf("BAD PARSING: memory allocation problem\n");
          break;
        }
      }

    } else {
      printf("BAD PASING: Got unexpected option\n");
      break;
    }
  }

  if (opt_id < argc || !append_char(parsed_opts, parsed_opts_size, '\0')) {
    printf("ARGS: %d %d\n", argc, opt_id);
    free_container(not_dashed, not_dashed_size);
    free(*parsed_opts);
    *not_dashed = NULL;
  }

  return *not_dashed;
}