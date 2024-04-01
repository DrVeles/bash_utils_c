# Bash Utilities

Описание проекта на русском [тут](README_RUS.md)

Implementation of basic Bash utilities in C.

## cat
Cat is one of the most commonly used commands in Unix-like operating systems. 

`cat [OPTION] [FILE]...`.

### cat Options

| No. | Options | Description |
| ------ | ------ | ------ |
| 1 | -b (GNU: --number-nonblank) | numbers only non-blank lines |
| 2 | -e assumes and -v (GNU only: -E is the same, but without -v) | also displays end-of-line characters as $ |
| 3 | -n (GNU: --number) | numbers all output lines |
| 4 | -s (GNU: --squeeze-blank) | compresses multiple adjacent blank lines |
| 5 | -t assumes and -v (GNU: -T is the same but without -v) | also displays tabs as ^I |


## grep

A utility for searching files using regular expressions.

`grep [options] template [file_name]`

### grep Options

|  No. | Options | Description |
| ------ | ------ | ------ |
| 1 | -e | template |
| 2 | -i | Ignores case differences. |
| 3 | -v | Inverts the meaning of matching. |
| 4 | -c | Outputs only the number of matching strings. |
| 5 | -l | Only displays matching files. |
| 6 | -n | Precedes each line of output with the line number of the input file. |
| 7 | -h | Outputs matching lines without preceding them with file names. |
| 8 | -s | Suppresses error messages about non-existent or unreadable files. |
| 9 | -f file | Retrieves regular expressions from a file. |
| 10 | -o | Prints only matching (non-empty) parts of a matching string. | 

Flags support pairwise combinations (e.g., `-iv`, `-in`).
