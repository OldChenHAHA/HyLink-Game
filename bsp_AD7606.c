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
float ADC_Values[CH_NUM];

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

// Software Poll
void AD7606_FetchValue(){

	int i = 0;
	uint16_t temp;
	float temp_ADC_Values[CH_NUM]={0};
	float max_ADC_Values[CH_NUM]={0};

	for (i=0; i<100; i++)
	{
		AD7606_CheckBusy();
		AD7606_StartConv();
		AD7606_CheckBusy();

		digitalWrite(CS, LOW);
		if (wiringPiSPIDataRW (SPI_CHANNEL_0, ADC_Bytes, CH_NUM * 2) == -1){
			printf("SPI failure: %s\n", strerror(errno));
		}
		digitalWrite(CS, HIGH);

		for(i = 0;i < CH_NUM; ++i){

			temp = ADC_Bytes[2 * i + 1] + (ADC_Bytes[2 * i] << 8);
			temp_ADC_Values[i] = 1.0 * temp * 10 / 65535;
			if(temp_ADC_Values[i] >= 5){
				temp_ADC_Values[i] = - (10 - temp_ADC_Values[i]);
			}
			if ( abs(temp_ADC_Values[i]) > abs(max_ADC_Values[i]) )
				max_ADC_Values[i] = temp_ADC_Values[i];
		}
	}
	for (i = 0; i < CH_NUM; i++)
		ADC_Values[i] = max_ADC_Values[i];

	#ifdef ADC_VALUE_DISP
	for (i=0; i< CH_NUM; i++)
		printf("%f | ", ADC_Values[i]);
	printf("\n");
	#endif

}
