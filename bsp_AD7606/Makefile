#编译器
CC     = gcc
#包含文件所在目录
INCLUDE = .
#编译选项
#CFLAGS = -O -Wall
CFLAGS = -g -Wall
#Object文件
OBJ    = test_AD7606.o bsp_AD7606.o
#-include <filename>
AD7606_test:$(OBJ)
	@echo Linking......
	@$(CC) -Wall -o AD7606_test $(OBJ) -lwiringPi
	@echo Complete!!!
test_AD7606.o:test_AD7606.cpp bsp.h
	@echo Compiling test_AD7606.cpp......
	@$(CC) -Wall -c test_AD7606.cpp -lwiringPi
	@echo OK!!!
bsp_AD7606.o:bsp_AD7606.cpp bsp_AD7606.h bsp.h
	@echo Compiling bsp_AD7606.cpp......
	@$(CC) -Wall -c bsp_AD7606.cpp -lwiringPi
	@echo OK!!!
.PHONY:cleanall cleanobj cleandiff
cleanall:cleanobj cleandiff
	@echo Cleaning all......
	@-rm AD7606_test
	@echo Complete!!!
cleanobj:
	@echo Cleaning Obj......
	@-rm *.o
	@echo Complete!!!
cleandiff:
	@echo Cleaning Diff......
	@-rm *.diff
	@echo Complete!!!
