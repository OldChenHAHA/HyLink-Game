#ifndef __BSP_H
#define __BSP_H

#define SPI_MODE_0 0u   //CPOL = 0, CPHA = 0, Clock idle low, data is clocked in on rising edge, output data (change) on falling edge
#define SPI_MODE_1 1u   //CPOL = 0, CPHA = 1, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
#define SPI_MODE_2 2u   //CPOL = 1, CPHA = 0, Clock idle high, data is clocked in on falling edge, output data (change) on rising edge
#define SPI_MODE_3 3u   //CPOL = 1, CPHA = 1, Clock idle high, data is clocked in on rising, edge output data (change) on falling edge

#define SPI_CHANNEL_0 0u
#define SPI_CHANNEL_1 1u

#endif