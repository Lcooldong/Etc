/*
 * GPIO_LEDtoSW_Polling.c
 *
 * Created: 2019-11-25 오후 5:46:05
 * Author : Hybus Technical Education & Design
 */

#define F_CPU	1474560UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "string.h"

/* USER CODE BEGIN Includes */
#include "main.h"
#include "Drv_LEDtoSW.h"
#include "Drv_GPIO.h"
/* USER CODE END Includes */


int main(void)
{
	DDRH=0xFF;
	PORTH=0x00;
	
	//Motor(Servo, Step, DC) 플로팅 현상 방지를 위한 IO 설정
	Protect_IO_Init();
	
	//LED & SW DDR,PORT Setting
	LEDtoSW_Init();
	
	//LED TEST
	for(int PinIndex = 0; PinIndex < 8; PinIndex++){
		GPIO_WritePin(&LED_PORTC, PinIndex, GPIO_PIN_TOGGLE);
		delay_ms(500);
	}
	for(int PinIndex = 0; PinIndex < 8; PinIndex++){
		GPIO_WritePin(&LED_PORTC, PinIndex, GPIO_PIN_TOGGLE);
		delay_ms(500);
	}
	while(1){
#if 0	
	/* Replace with your application code */
		//각각의 스위치 클릭 시 해당 LED 점멸
	SW_SHORT_Press_Event(SW_PINL & 0xFF);
		//버튼 클릭 시 리플 발생으로 인한 오동작 방지
	delay_ms(150);	
#else
	LED_PORTC = 0x00;
	delay_ms(1500);
	LED_PORTC = 0xFF;
	delay_ms(1500);
	
#endif
	}
}

