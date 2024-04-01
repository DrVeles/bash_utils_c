CC_FLAGGED = gcc -std=c11 -Wall -Werror -Wextra 
CLANG_COPY = cp ../../materials/linters/.clang-format ./ 
CLANGER = -clang-format -i *.c *.h

.PHONY : all build s21_grep clean rebuild

all : s21_grep 

build : s21_grep

s21_grep :
	$(CLANG_COPY)
	$(CLANGER)
	$(CC_FLAGGED) s21_grep.c -o s21_grep

clean :
	-rm s21_grep a.out *.o

rebuild: clean build