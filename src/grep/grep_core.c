#include <string.h>

#include "args.h"

extern int string_analyzer(char *string, char **templates, size_t t_size,
                           char ***matched_strings, size_t *c_size,
                           Options *opts);
extern void row_printer(FILE *stream, Options *opts, InitialData *data,
                        RowData *row_data);
extern void free_container(char ***str_container, size_t *size);

// главная функция утилиты: читаем файл построчно, анализируем строку, выводим
// нужные части с учетом всех опций
int scan_file(FILE *file, char *filename, InitialData *data, Options *opts) {
  int status = 0;

  char buf[2048];
  size_t row_num = 1, matched_quantity = 0;

  while (fgets(buf, sizeof(buf), file)) {
    if (buf[strlen(buf) - 1] ==
        '\n') {  // убираем перенос строки, чтобы работать только с исходной
                 // строкой, при выводе учитываем его
      buf[strlen(buf) - 1] = '\0';
    }

    char **matched_strings = NULL;
    size_t c_size = 0;

    if ((status = string_analyzer(buf, data->templates, data->templates_size,
                                  &matched_strings, &c_size, opts)) == -1) {
      break;
    }

    if (matched_strings) {
      if (opts->fnames_only) {
        fprintf(stdout, "%s\n", filename);
        free_container(&matched_strings, &c_size);
        break;
      } else if (!opts->rows_count) {
        RowData row_data = {.filename = filename,
                            .matched_strings = matched_strings,
                            .matched_str_size = c_size,
                            .row_num = row_num};
        row_printer(stdout, opts, data, &row_data);
      }
      ++matched_quantity;

      free_container(&matched_strings, &c_size);
    }

    ++row_num;
  }

  if (status != -1 && opts->rows_count && !opts->fnames_only) {
    char tmp[2048] = "";
    fprintf(stdout, "%s%ld\n",
            data->fs_to_scan_sz > 1 && !opts->no_fnames
                ? strcat(strcpy(tmp, filename), ":")
                : "",
            matched_quantity);
  }

  return status;
}