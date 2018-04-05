#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "bsp_AD7606.h"

uint8_t ADC_Bytes[CH_NUM*2];

void spiSetup(){
    if((wiringPiSPISetupMode(SPI_CHANNEL_0, SPI_CLK, SPI_MODE_2)) < 0){
        fprintf(stderr, "Can't open the SPI bus: %s\n", strerror (errno));
        exit(EXIT_FAILURE);
    }
}


void AD7606_Init(){
	spiSetup();
	AD7606_IOSet();
	AD7606_SetDF(0);                 //default:OS2 OS1 OS0 = 000
	AD7606_SetInputRange(0);         //default:range = -5 ~ +5
	AD7606_Reset();
}

//hook function,used to set input range.
void AD7606_SetInputRange(int _ucRange){

}
//hook function,used to set digital filter.
void AD7606_SetDF(uint8_t _ucMode){

}
//reset function
void AD7606_Reset(){
	digitalWrite(CS , HIGH);
	digitalWrite(CVA , HIGH);
	digitalWrite(CVB , HIGH);
 	/* AD7606 is high level reset，at least 50ns */
	digitalWrite(RST, LOW);
	//delayMicroseconds(1);
	digitalWrite(RST, HIGH);
	//delayMicroseconds(1);
	digitalWrite(RST, LOW);
}

void AD7606_IOSet(){
	pinMode(RST , OUTPUT);
	pinMode(CVA, OUTPUT);
	pinMode(CVB, OUTPUT);
	pinMode(CS  , OUTPUT);
	pinMode(BUSY, INPUT);
	pullUpDnControl(BUSY, PUD_UP);
	digitalWrite(RST, LOW);
	digitalWrite(CS , HIGH);
	digitalWrite(CVA , HIGH);
	digitalWrite(CVB , HIGH);
}

void AD7606_StartConv(){
	/* Conv in rising edge，at least 25ns  */
	digitalWrite(CVA, LOW);
	digitalWrite(CVB, LOW);
	//delayMicroseconds(1);
	digitalWrite(CVA, HIGH);
	digitalWrite(CVB, HIGH);
}

void AD7606_CheckBusy(){
 	while( digitalRead(BUSY) == 1 );
}

//Software Poll
void AD7606_FetchValue(){

	float ADC_Values[CH_NUM];
	//uint8_t ADC_Bytes[CH_NUM * 2];
	int i = 0;
	uint16_t temp;

	while(1){

	AD7606_CheckBusy();
	AD7606_StartConv();
	AD7606_CheckBusy();
	printf("fuck\n");

	digitalWrite(CS, LOW);
	if (wiringPiSPIDataRW (SPI_CHANNEL_0, ADC_Bytes, CH_NUM * 2) == -1){
		printf("SPI failure: %s\n", strerror(errno));
	}
	digitalWrite(CS, HIGH);

	for(i = 0;i < CH_NUM; ++i){

		temp = ADC_Bytes[2 * i + 1] + (ADC_Bytes[2 * i] << 8);
		ADC_Values[i] = 1.0 * temp * 10 / 65535;
		if(ADC_Values[i] >= 5){
			ADC_Values[i] = - (10 - ADC_Values[i]);
		}
		printf("| %f |\n", ADC_Values[i]);
	}

	delay(1);
	}

	//return ADC_Values;
}
