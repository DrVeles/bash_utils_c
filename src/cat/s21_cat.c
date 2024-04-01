#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

// flags -beEnstTuv for cat
typedef struct {
  bool s_flag;  // 1 priority - сжимает пустые строки в одну пустую строку
  bool b_flag;  // 2 priority - нумерует не пустые строки, приоритетне чем n
  bool n_flag;  // 3 priority - нумерует все строки
  bool e_flag;  // 4 priority - отображает символ конца строки
  bool v_flag;  // 4 priority - отображение непечатных символов в файле
  bool t_flag;  // 4 priority - отображает табуляции как: ^I
} type_flags;

void parsing(int argc, char *argv[], type_flags *flags);
void openAndPrinting(char *file_name, type_flags *flags);
char flagsProcessing(char current_symbol, char previos_symbol,
                     int *ptr_row_number, type_flags *flags);

int main(int argc, char *argv[]) {
  type_flags flags = {0};

  parsing(argc, argv, &flags);

  return 0;
}

// parsing flags function
void parsing(int argc, char *argv[], type_flags *flags) {
  int temp_opt = -1;
  static struct option long_opt[] = {{"number-nonblank", 0, 0, 'b'},
                                     {"number", 0, 0, 'n'},
                                     {"squeeze-blank", 0, 0, 's'},
                                     {0, 0, 0, 0}};

  while ((temp_opt = getopt_long(argc, argv, "+beEntTsuv", long_opt, NULL)) !=
         -1) {
    switch (temp_opt) {
      case 'b':
        flags->b_flag = true;
        break;
      case 'e':
        flags->e_flag = flags->v_flag = true;
        break;
      case 'E':
        flags->e_flag = true;
        break;
      case 'n':
        flags->n_flag = true;
        break;
      case 't':
        flags->t_flag = flags->v_flag = true;
        break;
      case 'T':
        flags->t_flag = true;
        break;
      case 's':
        flags->s_flag = true;
        break;
      case 'v':
        flags->v_flag = true;
        break;
      default:
        printf("usage: cat [-benstuv] [file ...]\n");
        return;
        break;
    }
  }
  for (int i = optind; i < argc; i++) {
    openAndPrinting(argv[i], flags);
  }
}

// open file and processing with flags every symbol, than printing
void openAndPrinting(char *file_name, type_flags *flags) {
  int row_number = 1;

  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    fprintf(stderr, "s21_cat: %s: No such file or directory\n", file_name);
  } else {
    char pre_previos_symbol = '\0';
    char previos_symbol = '\0';
    char current_symbol;  // renault symbol )))

    while ((current_symbol = fgetc(file)) != EOF) {
      if (flags->s_flag && current_symbol == '\n' && previos_symbol == '\n' &&
          pre_previos_symbol == '\n') {
        continue;
      }
      char processed_current_symbol =
          flagsProcessing(current_symbol, previos_symbol, &row_number, flags);
      pre_previos_symbol = previos_symbol;
      previos_symbol = current_symbol;
      putchar(processed_current_symbol);
    }
    fclose(file);
  }
}

// operation with every symbol with flags
char flagsProcessing(char current_symbol, char previos_symbol,
                     int *ptr_row_number, type_flags *flags) {
  if (flags->b_flag && (previos_symbol == '\n' || previos_symbol == '\0') &&
      current_symbol != '\n') {
    printf("%6d\t", *ptr_row_number);
    *ptr_row_number += 1;
  }
  if (flags->n_flag && !flags->b_flag &&
      (previos_symbol == '\n' || previos_symbol == '\0')) {
    printf("%6d\t", *ptr_row_number);
    *ptr_row_number += 1;
  }
  if (flags->e_flag && current_symbol == '\n') {
    printf("$");
  }
  if (flags->v_flag) {
    if ((current_symbol >= 0 && current_symbol <= 31 && current_symbol != 9 &&
         current_symbol != 10)) {
      current_symbol += 64;
      printf("^");
    } else if (current_symbol == 127) {
      current_symbol -= 64;
      printf("^");
    }
  }
  if (flags->t_flag && current_symbol == '\t') {
    printf("^");
    current_symbol = 'I';
  }
  return current_symbol;
}
