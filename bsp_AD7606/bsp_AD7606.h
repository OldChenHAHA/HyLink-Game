#ifndef __BSP_AD7606_H
#define __BSP_AD7606_H

#include <stdint.h>

#define CH_NUM 8u
#define	NUM_TIMES 100

#define RST 29
#define CVA 1
#define CVB 4
#define CS 27
#define BUSY 28

extern uint8_t ADC_val[CH_NUM * 2];

void AD7606_Init();
void AD7606_SetInputRange(int _ucRange);
void AD7606_SetDF(uint8_t _ucMode);
void AD7606_Reset();
void AD7606_IOSet();
void AD7606_StartConv();
bool AD7606_StartADC();

#endif