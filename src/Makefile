CC = g++
OBJS = Console.o Display.o lexer.o
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
	gcc -I$(INC) -MMD -c $<

lexer.c: lexer.l
	lex  --outfile=./lexer.c lexer.l

clean:
	rm $(OBJS) $(DEPS)