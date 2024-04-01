# CC_NOT_FLAGGED = gcc -std=c11 
CC_FLAGGED = gcc -std=c11 -Wall -Werror -Wextra 
CLANG_COPY = cp ../../materials/linters/.clang-format ./ 
CLANGER = clang-format -i *.c 
# LEAKER = leaks -atExit --
# CPPCHEKER = cppcheck --enable=all --suppress=missingIncludeSystem ./

# $(CLANGER)
.PHONY: all build s21_cat clean rebuild

all: s21_cat 

build: s21_cat

s21_cat:
	$(CLANG_COPY)
	$(CLANGER)
	$(CC_FLAGGED) s21_cat.c -o s21_cat

clean:
	-rm t.out s21_cat a.out 

rebuild: clean build