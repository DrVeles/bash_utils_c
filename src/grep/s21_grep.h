#ifndef _S21_GREP_H_
#define _S21_GREP_H_

#define _GNU_SOURCE

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// flags -eivclnhsfo for grep
typedef struct {
  bool i_flag;  // 1. ignore uppercase and lowercase
  bool v_flag;  // 2. invert match.
  bool c_flag;  // 3. output count of matching lines only.
  bool l_flag;  // Output matching files only.
  bool n_flag;  // Precede each matching line with a line number.
  bool e_flag;  // Pattern
  bool h_flag;  // Output matching lines without preceding them by file names.
  bool s_flag;  // Suppress error messages about nonexist files.
  bool o_flag;  // Output the matched parts of a matching line.
  bool f_flag;  // Take regexes from a file.
} type_flags;

typedef struct {
  bool is_print_line;     // need print line?
  bool is_one_file_only;  // no more files
  int match_counter;      // all line with match
  int all_line_counter;   // counting all lines in file
  char *ptr_to_filename;  // filename to open or printing
  char *ptr_to_buffer;    // buffer line from opened file
  bool file_not_exist;    // file not exist?
  int reti;               // regex result of comparison
} type_parameters;

void parsing(int argc, char *argv[], type_flags *flags,
             type_parameters *parameters);
void afterParse(int argc, char *argv[], char *pattern, type_flags *flags,
                type_parameters *parameters);

void openAndPrinting(char *pattern, type_flags *flags,
                     type_parameters *parameters);
void processingFlags(type_flags *flags, type_parameters *parameters);
void printingIfSinglefile(type_flags *flags, type_parameters *parameters);
void printingIfMultifile(type_flags *flags, type_parameters *parameters);
void lchFlagsPrinting(type_flags *flags, type_parameters *parameters);
void oFlagPrinting(regex_t regex, type_flags *flags,
                   type_parameters *parameters);
char *fFlagProcessing(char *pattern, char *optarg);

#endif