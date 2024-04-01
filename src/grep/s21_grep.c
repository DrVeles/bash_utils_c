#include "s21_grep.h"

int main(int argc, char *argv[]) {
  type_flags flags = {0};
  type_parameters parameters = {0};

  parsing(argc, argv, &flags, &parameters);

  return 0;
}

// parsing using getopt.h
void parsing(int argc, char *argv[], type_flags *flags,
             type_parameters *parameters) {
  int temp_opt = -1;
  char *pattern = calloc(2, sizeof(char));
  while ((temp_opt = getopt_long(argc, argv, "e:ivclnhsf:o", 0, NULL)) != -1) {
    switch (temp_opt) {
      case 'e':
        flags->e_flag = true;
        pattern = (realloc(
            pattern, (sizeof(pattern) + sizeof(optarg) + 2 * sizeof(char))));
        if (pattern != NULL) {
          strcat(pattern, optarg);
          strcat(pattern, "|");
        }
        break;
      case 'i':
        flags->i_flag = true;
        break;
      case 'v':
        flags->v_flag = true;
        break;
      case 'c':
        flags->c_flag = true;
        flags->n_flag = flags->o_flag = false;
        break;
      case 'l':
        flags->l_flag = true;
        flags->o_flag = false;
        break;
      case 'n':
        if (!flags->c_flag) flags->n_flag = true;
        break;
      case 'h':
        flags->h_flag = true;
        break;
      case 's':
        flags->s_flag = true;
        break;
      case 'f':
        flags->f_flag = true;
        pattern = fFlagProcessing(pattern, optarg);
        break;
      case 'o':
        if (!flags->l_flag && !flags->c_flag) flags->o_flag = true;
        break;
    }
  }
  afterParse(argc, argv, pattern, flags, parameters);
}

// some operations after parsing
void afterParse(int argc, char *argv[], char *pattern, type_flags *flags,
                type_parameters *parameters) {
  if (flags->e_flag || flags->f_flag) {
    if (pattern[strlen(pattern) - 1] == '|')
      pattern[strlen(pattern) - 1] = '\0';
  } else if (!flags->f_flag) {
    pattern = realloc(
        pattern, (sizeof(pattern) + sizeof(argv[optind]) + 2 * sizeof(char)));
    if (pattern) {
      strcat(pattern, argv[optind]);
    } else
      fprintf(stderr, "Memory not allocated\n");
  }
  if (((!flags->f_flag && !flags->e_flag) && argv[optind + 2] == NULL) ||
      ((flags->f_flag || flags->e_flag) && argv[optind + 1] == NULL)) {
    parameters->is_one_file_only = true;
  } else {
    parameters->is_one_file_only = false;
  }
  for (int i = (flags->e_flag || flags->f_flag) ? optind : optind + 1; i < argc;
       i++) {
    parameters->ptr_to_filename = argv[i];
    parameters->all_line_counter = 1;
    parameters->match_counter = 0;
    parameters->file_not_exist = 0;
    openAndPrinting(pattern, flags, parameters);

    if ((flags->c_flag || flags->l_flag) && !parameters->file_not_exist)
      lchFlagsPrinting(flags, parameters);
  }
  free(pattern);
}

// opening file to searcing in it
void openAndPrinting(char *pattern, type_flags *flags,
                     type_parameters *parameters) {
  char *buffer = NULL;
  size_t buffer_size = 0;
  int readed = 0;
  FILE *file = fopen(parameters->ptr_to_filename, "r");
  if (file == NULL) {
    parameters->file_not_exist = true;
    if (!flags->s_flag)
      fprintf(stderr, "s21_grep: %s: No such file or directory\n",
              parameters->ptr_to_filename);
  } else {
    int i = 0;
    regex_t regex;
    regmatch_t match[1];

    if (flags->i_flag) {
      regcomp(&regex, pattern, REG_ICASE);
    } else {
      regcomp(&regex, pattern, REG_EXTENDED);
    }
    while ((readed = getline(&buffer, &buffer_size, file)) != -1) {
      if ((buffer[strlen(buffer) - 1]) != '\n') strcat(buffer, "\n");
      parameters->ptr_to_buffer = buffer;
      parameters->reti = regexec(&regex, buffer, 0, match, 0);
      processingFlags(flags, parameters);
      if (flags->o_flag) oFlagPrinting(regex, flags, parameters);
      if (parameters->is_one_file_only || flags->h_flag) {
        printingIfSinglefile(flags, parameters);
      } else if (!parameters->is_one_file_only && !flags->h_flag) {
        printingIfMultifile(flags, parameters);
      }
      parameters->reti = 0;
      parameters->all_line_counter++;
      i++;
    }
    regfree(&regex);
  }
  fclose(file);
  free(buffer);
}

// some logic processing flags and parameters
void processingFlags(type_flags *flags, type_parameters *parameters) {
  if (flags->v_flag)
    parameters->reti = !parameters->reti;  // flag v implementation

  if (!parameters->reti) {
    parameters->is_print_line = true;
    parameters->match_counter++;
  } else {
    parameters->is_print_line = false;
  }
  if (flags->c_flag || flags->l_flag) {
    parameters->is_print_line = false;
  }
}

// logic -l -c -h flags and printing them
void lchFlagsPrinting(type_flags *flags, type_parameters *parameters) {
  if (flags->c_flag && !flags->l_flag && parameters->is_one_file_only) {
    fprintf(stdout, "%d\n", parameters->match_counter);
  } else if (flags->c_flag && flags->l_flag && !flags->h_flag &&
             parameters->match_counter > 0 && parameters->is_one_file_only) {
    fprintf(stdout, "%d\n", 1);
  } else if (flags->c_flag && flags->l_flag && !flags->h_flag &&
             parameters->match_counter > 0) {
    fprintf(stdout, "%s:%d\n", parameters->ptr_to_filename, 1);
  } else if (flags->c_flag && flags->l_flag && flags->h_flag &&
             parameters->match_counter > 0) {
    fprintf(stdout, "%d\n", 1);
  } else if (flags->c_flag && !flags->h_flag) {
    fprintf(stdout, "%s:%d\n", parameters->ptr_to_filename,
            parameters->match_counter);
  } else if (flags->c_flag && flags->h_flag) {
    fprintf(stdout, "%d\n", parameters->match_counter);
  } else if (flags->l_flag && flags->h_flag && parameters->match_counter > 0) {
    fprintf(stdout, "%s\n", parameters->ptr_to_filename);
  }
  if (flags->l_flag && !flags->h_flag && parameters->match_counter > 0) {
    fprintf(stdout, "%s\n", parameters->ptr_to_filename);
  } else if (flags->l_flag && flags->c_flag && flags->h_flag &&
             parameters->match_counter > 0) {
    fprintf(stdout, "%s\n", parameters->ptr_to_filename);
  }
}

// working with -o flag
void oFlagPrinting(regex_t regex, type_flags *flags,
                   type_parameters *parameters) {
  regmatch_t matches[1];
  int start_index = 0;

  if (!flags->v_flag) {
    parameters->is_print_line = false;
    if ((!parameters->reti && !parameters->is_one_file_only &&
         !flags->h_flag) ||
        (!parameters->reti && flags->l_flag)) {
      fprintf(stdout, "%s:", parameters->ptr_to_filename);
    }
    if (!parameters->reti && flags->n_flag) {
      fprintf(stdout, "%d:", parameters->all_line_counter);
    }
    while (!regexec(&regex, parameters->ptr_to_buffer + start_index, 1, matches,
                    0)) {
      fprintf(stdout, "%.*s\n", (int)(matches[0].rm_eo - matches[0].rm_so),
              parameters->ptr_to_buffer + start_index + matches[0].rm_so);
      start_index += matches[0].rm_eo;
      parameters->is_print_line = false;
    }
  }
}

// processing with -f flag
char *fFlagProcessing(char *pattern, char *optarg) {
  char *buffer = NULL;
  size_t buffer_size = 0;
  int readed = 0;
  FILE *file = fopen(optarg, "r");

  if (file == NULL) {
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", optarg);
  } else {
    while ((readed = getline(&buffer, &buffer_size, file)) != -1) {
      int buffer_length = strlen(buffer);
      if (buffer[buffer_length - 1] == '\n') {
        buffer[buffer_length - 1] = '|';
      }
      pattern = realloc(pattern,
                        (sizeof(pattern) + sizeof(buffer) + 2 * sizeof(char)));
      if (pattern != NULL) {
        strcat(pattern, buffer);
      }
    }
  }

  fclose(file);
  free(buffer);
  return pattern;
}

// print for single file (without -l -c -h flags)
void printingIfSinglefile(type_flags *flags, type_parameters *parameters) {
  if (flags->n_flag && parameters->is_print_line) {
    fprintf(stdout, "%d:%s", parameters->all_line_counter,
            parameters->ptr_to_buffer);
  } else if (parameters->is_print_line) {
    fprintf(stdout, "%s", parameters->ptr_to_buffer);
  }
}

// print for multi file (without -l -c -h flags)
void printingIfMultifile(type_flags *flags, type_parameters *parameters) {
  if (flags->n_flag && parameters->is_print_line) {
    fprintf(stdout, "%s:%d:%s", parameters->ptr_to_filename,
            parameters->all_line_counter, parameters->ptr_to_buffer);
  } else if (parameters->is_print_line) {
    fprintf(stdout, "%s:%s", parameters->ptr_to_filename,
            parameters->ptr_to_buffer);
  }
}
