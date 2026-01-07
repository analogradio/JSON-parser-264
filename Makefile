CC=gcc
CFLAGS=-g -std=c11 -Wall -Wshadow -Wvla -Werror -pedantic 
BASE_NAME=json
SRC_C=$(BASE_NAME).c
TEST_C=test_$(SRC_C)
ASG_NICKNAME=HW13
SRC_H=$(BASE_NAME).h clog.h miniunit.h
SUBMIT_FILES=$(SRC_C) $(TEST_C) clog.h miniunit.h Makefile 
EXECUTABLE=test_$(BASE_NAME)
EXECUTABLE_GCOV=$(EXECUTABLE)_gcov
CFLAGS_GCOV=$(CFLAGS) -fprofile-arcs -ftest-coverage
SHELL=/bin/bash

submit: $(SUBMIT_FILES)
	264submit $(ASG_NICKNAME) $(SUBMIT_FILES)

test:  $(EXECUTABLE) 
	./$(EXECUTABLE)
	 valgrind ./$(EXECUTABLE)

pretest: submit 
	264test $(ASG_NICKNAME) 

clean: 
	rm -f $(EXECUTABLE) *.c.gcov *.gcda *.gcno

$(EXECUTABLE): $(SRC_C) $(TEST_C) $(SRC_H)
	#$(CC) -o test_$(BASE_NAME) $(SRC_C) $(TEST_C) $(CFLAGS)

coverage: $(SRC_C) $(TEST_C)
	$(CC) $(CFLAGS_GCOV) $(SRC_C) $(TEST_C) -o $(EXECUTABLE_GCOV)
	./$(EXECUTABLE_GCOV)
	gcov -f $(SRC_C)

.PHONY: submit test pretest clean coverage
