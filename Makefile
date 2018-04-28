
FILES = bsp_AD7606.c bsp_AD7606.h bsp_LCD12864.c bsp_LCD12864.h

all:$(FILES)
	@echo Compiling Raspberrypi Server ......
	@$(CC) -Wall -o ServerApp server.c $(FILES) -l wiringPi
	@echo Compile Done !

.PHONY:clean
clean:
	@echo Cleaning all ......
	@-rm ServerApp
	@echo CLean Done !
