CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra -g
GCOV_FLAGS=-fprofile-arcs -ftest-coverage
LINUX_FLAGS=-lsubunit -lrt -lpthread -lm

CHECKS=$(wildcard ./tests/test_*.check)
TEST_SRC=$(wildcard ./tests/test_*.c)

DIR=./another/ \
	./arithmetic/ \
	./common/bitwise_operators/ \
	./common/bit_calls/ \
	./common/scale_operators/ \
	./common/other/ \
	./comparison/ \
	./convert/
SRC=$(foreach dir,$(DIR),$(wildcard $(dir)*.c))
OBJ=$(patsubst %.c,%.o,$(SRC))

MSG=Minor bug fixes
BRANCH=$(shell git branch --show-current)

OS := $(shell uname -s)

ifeq ($(OS),Linux)
  OPEN_CMD = xdg-open
endif
ifeq ($(OS),Darwin)
	OPEN_CMD = open
endif

all:
	make s21_decimal.a
	make test_s21_decimal.c
	make test

rebuild:
	make clean
	make all

style:
	clang-format -n $(SRC) *.h

clean:
	make clean_obj
	make clean_test
	make clean_gcov
	rm -rf *.a *.dSYM ./tests/test_*.c


test_s21_decimal.c: $(CHECKS)
	checkmk clean_mode=1 $(CHECKS) > ./tests//$@

s21_decimal.a: $(OBJ)
	ar rc $@ $(OBJ)
	ranlib $@
	cp $@ lib$@
	make clean_obj

clean_obj:
	rm -rf $(OBJ)

test:
ifeq ($(OS), Darwin)
	$(CC) $(CFLAGS) ${TEST_SRC} s21_decimal.a -o unit_test -lcheck -lm
else
	$(CC) $(CFLAGS) ${TEST_SRC} s21_decimal.a -o unit_test -lcheck $(LINUX_FLAGS)
endif
	./unit_test

clean_test:
	rm -rf unit_test

# If lcov cause errors add "--filter range" after --no-external flag, works only in MAC
gcov_report:
ifeq ($(OS), Darwin)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) ${TEST_SRC} ${SRC} -o gcov_test -lcheck -lm
else
	$(CC) $(CFLAGS) $(GCOV_FLAGS) ${TEST_SRC} ${SRC} -o gcov_test -lcheck $(LINUX_FLAGS)
endif
	./gcov_test
	lcov -t "stest" -o s21_test.info --no-external -c -d .
	genhtml -o report s21_test.info
	$(OPEN_CMD) ./report/index.html

clean_gcov:
	rm -rf *.gcda *.gcno *.info report gcov_test

push:
	git add .
	git commit -m "$(BRANCH): $(MSG)"
	git push origin $(BRANCH)

pull:
	git pull origin $(BRANCH)

gitback:
	git stash save --keep-index