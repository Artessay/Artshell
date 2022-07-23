# 程序：命令行编辑器
# 作者：邱日宏 3200105842

CC = g++
INC = ./inc
SRC = ./src
LIB = ./lib
CFLAG = -I$(INC) -L$(LIB) -O3 -Wall -MMD
OBJS = myshell.o
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
	
clean:
	rm $(OBJS) $(DEPS) $(RUN)

cleanall:
	rm $(OBJS) $(DEPS) $(RUN) $(LIB)\lib$(LIBS).a
	make clean -C $(SRC)