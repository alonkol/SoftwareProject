#This is a comment: Macros are fun
CC = gcc
OBJS = main.o MathIsFun.o
EXEC = ex1
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
main.o: main.c MathIsFun.h
	$(CC) $(COMP_FLAG) -c $*.c
MathIsFun.o: MathIsFun.h MathIsFun.c
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
