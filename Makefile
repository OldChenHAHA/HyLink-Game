#编译器
CC     = gcc
#包含文件所在目录
INCLUDE = .
#编译选项
#CFLAGS = -O -Wall
CFLAGS = -g -Wall
#Object文件
OBJ    = main.o bsp_AD7606.o bsp_LCD12864.o
#-include <filename>
RaspberryApp:$(OBJ)
	@echo Linking......
	@$(CC) -Wall -o RaspberryApp $(OBJ) -lwiringPi
	@echo Complete!!!
main.o:main.c
	@echo Compiling main.c......
	@$(CC) -Wall -c main.c -lwiringPi
	@echo OK!!!

bsp_AD7606.o:bsp_AD7606.c bsp_AD7606.h
	@echo Compiling bsp_AD7606.c......
	@$(CC) -Wall -c bsp_AD7606.c -lwiringPi
	@echo OK!!!
bsp_LCD12864.o:bsp_LCD12864.c bsp_LCD12864.h
	@echo Compiling bsp_LCD12864.c......
	@$(CC) -Wall -c bsp_LCD12864.c -lwiringPi
	@echo OK!!!


.PHONY:cleanall cleanobj cleandiff
cleanall:cleanobj cleandiff
	@echo Cleaning all......
	@-rm RaspberryApp
	@echo Complete!!!
cleanobj:
	@echo Cleaning Obj......
	@-rm *.o
	@echo Complete!!!
cleandiff:
	@echo Cleaning Diff......
	@-rm *.diff
	@echo Complete!!!
