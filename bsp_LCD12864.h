#ifndef __BSP_LCD12864_H__
#define __BSP_LCD12864_H__

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "wiringPi.h"
#include "iconv.h"

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

extern char u2g_out[255];

//编码转换 UTF-8 -> GB2312
int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen);
int u2g(char *inbuf,int inlen,char *outbuf,int outlen);
//总线写入
void bus_write(unsigned char data);
//检查lcd忙状态
void chk_busy();
//写命令
void WriteCmd_LCD12864(unsigned char cmdcode);
//写数据
void WriteData_LCD12864(unsigned char Dispdata);
//向LCD指定位置发送一个字符串,长度64字符之内。
void WriteWord_LCD12864(unsigned char a,unsigned char *d);
//向LCD发送一屏字符串,长度64字符之内。
void WriteWord_LCD12864_2(unsigned char *d);
//初始化LCD屏
void Init_LCD12864(void);
//清除屏幕
void Clear_LCDScreen();

#endif
