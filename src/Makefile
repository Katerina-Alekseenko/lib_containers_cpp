CC = g++
TEST_LIBS = -lgtest -lgtest_main
ifeq ($(shell uname -s), Linux)
	TEST_LIBS += -lsubunit -lm -pthread
endif
CFLAGS = -std=c++17 -Wall -Werror -Wextra -Wno-sign-compare
SRC_TEST_DIR = tests/
SRC_TEST = $(wildcard $(SRC_TEST_DIR)*.cpp)

GCOV_FLAGS = -fprofile-arcs -ftest-coverage
LCOV = lcov
GENHTML = genhtml

all: style test

test: clean
	$(CC) $(CFLAGS) $(SRC_TEST) -o test $(TEST_LIBS)
	./test
	rm -rf tests/*.o

style:
	clang-format --style=google -i *.h
	clang-format --style=google -i tests/*.cpp

test_leaks:
	$(CC) $(CFLAGS) $(SRC_TEST) -o test $(TEST_LIBS)
	rm -rf tests/*.o
ifeq ($(shell uname -s), Linux)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./test
else
	leaks --atExit -- ./test
endif

coverage: clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SRC_TEST) -o test $(TEST_LIBS)
	./test
	$(LCOV) --capture --directory . --output-file coverage.info
	$(LCOV) --remove coverage.info '/usr/include/*' 'tests/*' --output-file coverage.info
	$(GENHTML) coverage.info --output-directory coverage
	rm -f *.gcno *.gcda coverage.info
	@echo "Coverage report generated at coverage/index.html"
	@xdg-open coverage/index.html 2>/dev/null || open coverage/index.html 2>/dev/null || \
	echo "Could not open the report automatically. Please open file://$(CURDIR)/coverage/index.html manually"

clean:
	rm -rf *.o tests/*.o test *.gcno *.gcda *.gcov coverage.info coverage

.PHONY: all clean test style test_leaks coverage