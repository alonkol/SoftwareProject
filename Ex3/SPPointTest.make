CC = gcc
OBJS = sp_point_unit_test.o SPPoint.o
EXEC = sp_point_unit_test
TESTS_DIR = ./unit_tests
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
sp_point_unit_test.o: $(TESTS_DIR)/sp_point_unit_test.c $(TESTS_DIR)/unit_test_util.h SPPoint.h
	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
SPPoint.o: SPPoint.c SPPoint.h 
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
