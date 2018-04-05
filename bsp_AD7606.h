#ifndef __BSP_AD7606_H__
#define __BSP_AD7606_H__

#include <stdint.h>

// this is for debug
#define ADC_VALUE_DISP

#define CH_NUM 8u
#define	NUM_TIMES 100

/* =========================
    Repi board PIN config
===========================*/
#define RST 29
#define CVA 1
#define CVB 4
#define CS 27
#define BUSY 28

/* =========================
    SPI CONFIG
===========================*/
#define SPI_MODE_0 0u   //CPOL = 0, CPHA = 0, Clock idle low, data is clocked in on rising edge, output data (change) on falling edge
#define SPI_MODE_1 1u   //CPOL = 0, CPHA = 1, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
#define SPI_MODE_2 2u   //CPOL = 1, CPHA = 0, Clock idle high, data is clocked in on falling edge, output data (change) on rising edge
#define SPI_MODE_3 3u   //CPOL = 1, CPHA = 1, Clock idle high, data is clocked in on rising, edge output data (change) on falling edge

#define SPI_CHANNEL_0 0u
#define SPI_CHANNEL_1 1u

#define SPI_CLK 5000000

void spiSetup();
void AD7606_Init();
void AD7606_SetInputRange(int _ucRange);
void AD7606_SetDF(uint8_t _ucMode);
void AD7606_Reset();
void AD7606_IOSet();
void AD7606_StartConv();
void AD7606_CheckBusy();
float * AD7606_FetchValue();

#endif
