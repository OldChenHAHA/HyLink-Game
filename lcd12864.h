#ifndef __LCD_12864___
#define __LCD_12864___

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>
#include <iconv.h> 
 
#define LCD_RS 6 //显示屏控制线
#define LCD_RW 5
#define LCD_EN 2
 
#define D1 30 //显示屏数据线
#define D2 21
#define D3 22
#define D4 23
#define D5 24
#define D6 25
#define D7 26
#define D8 0
 
char u2g_out[255]; 

#endif