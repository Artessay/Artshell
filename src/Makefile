CC = g++
OBJS = common.o Console.o Display.o Executor.o lexer.o myshell.o Parser.o
INC = ../inc
OUT = ../lib
CFLAG = -I$(INC) -O3 -Wall -MMD
DEPS = $(OBJS:.o=.d)
LIB = myshell

lib$(LIB).a: $(OBJS)
	ar -rcs $(OUT)/$@ $^

-include $(DEPS)

%.o: %.cpp
	$(CC) $(CFLAG) -c $<

%.o: %.c
	gcc -I$(INC) -w -MMD -c $<

lexer.c: lexer.l
	lex --header-file=../inc/lexer.h --outfile=./lexer.c lexer.l

clean:
	-rm $(OBJS) $(DEPS)
	-rm ../inc/lexer.h lexer.c