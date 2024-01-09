#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void default_printing(char ch) { putchar(ch); }

void print_nonprinting(char ch) {
  if (ch < -96) {
    printf("M-^%c", ch + 192);
  } else if (ch < 0) {
    printf("M-%c", ch + 128);
  } else if (ch == 9 || ch == 10) {
    putchar(ch);
  } else if (ch < 32) {
    printf("^%c", ch + 64);
  } else if (ch < 127) {
    putchar(ch);
  } else {
    printf("^?");
  }
}

void number(FILE *file, size_t *abs_offset) {
  static bool same_line = false;
  char buf[2048];
  while (fgets(buf, sizeof(buf), file)) {
    if (same_line) {
      printf("%s", buf);
      same_line = false;
    } else {
      printf("%6ld\t%s", *abs_offset, buf);
    }
    if (buf[strlen(buf) - 1] != '\n') {
      same_line = true;
    } else {
      ++(*abs_offset);
    }
  }
  fseek(file, 0, SEEK_SET);
}

void number_nonblank(FILE *file, size_t *abs_offset) {
  static bool same_line = false;
  char buf[2048];
  while (fgets(buf, sizeof(buf), file)) {
    if (same_line) {
      printf("%s", buf);
      same_line = false;
    } else {
      (strlen(buf) == 1 && buf[0] == '\n')
          ? printf("%s", buf)
          : printf("%6ld\t%s", *abs_offset, buf);
    }

    if (buf[strlen(buf) - 1] != '\n') {
      same_line = true;
    } else if (strlen(buf) != 1) {
      ++(*abs_offset);
    }
  }
  fseek(file, 0, SEEK_SET);
}

void show_special(FILE *file, char special, const char *show_as,
                  void (*printer)(char)) {
  int symbol;
  while ((symbol = fgetc(file)) != EOF) {
    if (symbol == special) {
      printf("%s", show_as);
      continue;
    }
    printer(symbol);
  }
  fseek(file, 0, SEEK_SET);
}

void show_nonprinting(FILE *file) {
  int symbol;
  while ((symbol = fgetc(file)) != EOF) {
    print_nonprinting(symbol);
  }
  fseek(file, 0, SEEK_SET);
}

void squeeze_blank(FILE *file) {
  char buf[2048];
  bool was_blank = false;
  while (fgets(buf, sizeof(buf), file)) {
    if (strlen(buf) == 1) {
      if (!was_blank) {
        was_blank = true;
        printf("%s", buf);
      }
    } else {
      was_blank = false;
      printf("%s", buf);
    }
  }
  fseek(file, 0, SEEK_SET);
}