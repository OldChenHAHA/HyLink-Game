#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "bsp_AD7606.h"
#include "bsp.h"

static int spiCLK = 5000000;
int myFd;

void spiSetup(int speed){
    if((myFd = wiringPiSPISetupMode(SPI_CHANNEL_0, speed, SPI_MODE_2)) < 0){
        fprintf(stderr, "Can't open the SPI bus: %s\n", strerror (errno));
        exit(EXIT_FAILURE);
    }
}

int main(){
	int i = 0;
	wiringPiSetup();
	spiSetup(spiCLK);
	AD7606_Init();
	printf("+----------+----------+----------+----------+----------+----------+----------+----------+\n");
    printf("|    V1    |    V2    |    V3    |    V4    |    V5    |    V6    |    V7    |    V8    |\n");
    printf("+----------+----------+----------+----------+----------+----------+----------+----------+\n");
	while(true){
		if(AD7606_StartADC()){
			for(i = 0;i < CH_NUM;++i){
				//printf("V%d:0x%02x%02x\n", (i + 1), ADC_val[2 * i], ADC_val[2 * i + 1]);
				uint16_t tmp;
				float disp;
				tmp = ADC_val[2 * i + 1] + (ADC_val[2 * i] << 8);
				disp = 1.0 * tmp * 10 / 65535; 
				if(disp >= 5){
					disp = 10 - disp;
					printf ("|-%f ", disp);
				} 
				else{
					printf ("| %f ", disp);
				}
			}
			printf("|\n");
            printf ("+----------+----------+----------+----------+----------+----------+----------+----------+\n") ;
		}
		else{
			fprintf(stderr, "Smaple Err: %s\n", strerror (errno));
		}
		delay(1000);
	}
}