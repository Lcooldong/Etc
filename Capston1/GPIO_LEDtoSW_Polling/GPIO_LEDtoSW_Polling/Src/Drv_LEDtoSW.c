/*
 * Drv_LEDtoSW.c
 *
 * Created: 2019-07-15 오후 2:12:37
 * Author : Hybus Technical Education & Design 
 */ 

#define F_CPU	14745600UL

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


void LEDtoSW_Init()
{	
	//LED_DDR = 0xFF;
	LED_DDRC	= (1 << LED_PORTC0) | (1 << LED_PORTC1) | (1 << LED_PORTC2) | (1 << LED_PORTC3)
				| (1 << LED_PORTC4) | (1 << LED_PORTC5) | (1 << LED_PORTC6) | (1 << LED_PORTC7);
	
	//LED_PORT = 0x00;
	LED_PORTC	= (0 << LED_PORTC0) | (0 << LED_PORTC1) | (0 << LED_PORTC2) | (0 << LED_PORTC3)
				| (0 << LED_PORTC4) | (0 << LED_PORTC5) | (0 << LED_PORTC6) | (0 << LED_PORTC7);
		
	//SW_DDR = 0x00;
	SW_DDRL		= (0 << SW_PINL0) | (0 << SW_PINL1) | (0 << SW_PINL2) | (0 << SW_PINL3)
				| (0 << SW_PINL4) | (0 << SW_PINL5) | (0 << SW_PINL6) | (0 << SW_PINL7);
	
	//SW_PIN = 0x00;
	SW_PINL		= (0 << SW_PINL0) | (0 << SW_PINL1) | (0 << SW_PINL2) | (0 << SW_PINL3)
				| (0 << SW_PINL4) | (0 << SW_PINL5) | (0 << SW_PINL6) | (0 << SW_PINL7);
}

void SW_SHORT_Press_Event(uint8_t swState)
{
	switch(swState)
	{
		case 0x01 :		//0b 0000 0001
		GPIO_WritePin(&LED_PORTC, LED_PORTC0, GPIO_PIN_TOGGLE);
		break;
		case 0x02 :		//0b 0000 0010
		GPIO_WritePin(&LED_PORTC, LED_PORTC1, GPIO_PIN_TOGGLE);
		break;
		case 0x04 :		//0b 0000 0100
		GPIO_WritePin(&LED_PORTC, LED_PORTC2, GPIO_PIN_TOGGLE);
		break;
		case 0x08 :		//0b 0000 1000
		GPIO_WritePin(&LED_PORTC, LED_PORTC3, GPIO_PIN_TOGGLE);
		break;
		case 0x10 :		//0b 0001 0000
		GPIO_WritePin(&LED_PORTC, LED_PORTC4, GPIO_PIN_TOGGLE);
		break;
		case 0x20 :		//0b 0010 0000
		GPIO_WritePin(&LED_PORTC, LED_PORTC5, GPIO_PIN_TOGGLE);
		break;
		case 0x40 :		//0b 0100 0000
		GPIO_WritePin(&LED_PORTC, LED_PORTC6, GPIO_PIN_TOGGLE);
		break;
		case 0x80 :		//0b 1000 0000
		GPIO_WritePin(&LED_PORTC, LED_PORTC7, GPIO_PIN_TOGGLE);
		break;
	}
}