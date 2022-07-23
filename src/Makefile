CC = g++
OBJS = Console.o Display.o
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

clean:
	rm $(OBJS) $(DEPS)