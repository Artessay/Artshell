# 程序：命令行解释器
# 作者：邱日宏 3200105842

CC = g++
INC = ./inc
SRC = ./src
LIB = ./lib
CFLAG = -I$(INC) -L$(LIB) -O3 -Wall -MMD
OBJS = main.o
LIBS = myshell
DEPS = $(OBJS:.o=.d)
RUN = myshell

$(RUN): $(OBJS) lib$(LIBS).a
	$(CC) $(CFLAG) $< -l$(LIBS) -o $@
	
lib$(LIBS).a:
	make -C $(SRC)
	
-include $(DEPS)

.cpp.o:
	$(CC) $(CFLAG) -c -o $@ $<

debug: lib$(LIBS).a
	-rm $(OBJS) $(DEPS) $(RUN)
	$(CC) $(CFLAG) -D_DEBUG_ -c -o main.o main.cpp
	$(CC) $(CFLAG) $(OBJS) -l$(LIBS) -o myshell

clean:
	-rm $(OBJS) $(DEPS) $(RUN)

cleanall:
	-rm $(OBJS) $(DEPS) $(RUN) $(LIB)/lib$(LIBS).a
	make clean -C $(SRC)