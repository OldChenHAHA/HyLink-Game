/*

*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include <bsp_LCD12864.h>
#include <bsp_AD7606.h>

#define ADC_VALUE_DISP

int main(int argc, char const *argv[])
{
	float * adc_val_ptr;

	wiringPiSetup();
	AD7606_Init();
	Init_LCD12864();

	Clear_LCDScreen();
	WriteWord_LCD12864(0x80,"Hello LCD12864");

	while (1) {
		adc_val_ptr = AD7606_FetchValue();

#ifdef ADC_VALUE_DISP
		for(int i = 0;i < CH_NUM; ++i){
			printf("| %f ", adc_val_ptr[i]);
		}
		printf("\n");
#endif

	delay(1);

	}


	return 0;
}
