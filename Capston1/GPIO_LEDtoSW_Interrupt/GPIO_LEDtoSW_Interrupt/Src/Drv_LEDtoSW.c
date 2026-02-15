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
#include "Drv_UART.h"
#include "App_DotMatrix.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Variable */
SWITCH_TypeDef hSwtich;
/* USER CODE END Variable */


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

uint8_t SW_StatusPin()
{
	uint8_t status = 0;
	status = PINL & 0xff;
	return status;
}


void SW_READ(SWITCH_TypeDef *hSW)
{
	hSW->State = SW_StatusPin();	//스위치 상태 정보값
	
	if(hSW->State != 0x00){			//first botton click
		if(!hSW->Pressed){
			hSW->Pressed = HIGH;
			hSW->Delaycount = 0;
			hSW->Value = 0;
			hSW->Value = hSW->State;
		}
	}
	else{
		if(hSW->Delaycount >= SHORTPRESS && hSW->Delaycount < LONGPRESS){	//	>=50ms, <2000ms
			SW_SHORT_Press_Event(hSW->Value);
		}
		if(hSW->Delaycount >= LONGPRESS){		//	>= 2000ms
			/* function input START */
			SW_LONG_Press_Event();
			/* funciton input END */
		}
		hSW->Delaycount = 0;
		hSW->Pressed = LOW;
	}
}

//해당 함수는 Drv_Timer0.c 파일의  ISR(TIMER0_COMPB_vect) 함수에 적용
void SW_TICK_FUNC(SWITCH_TypeDef *hSW){
	if(hSW->Pressed == HIGH){
		hSW->Delaycount++;
	}
	else{
		hSW->Delaycount = 0;
	}
}

void SW_SHORT_Press_Event(uint8_t swState)
{
	switch(swState)
	{
		case 0x01 :		//0b 0000 0001
			GPIO_WritePin(&LED_PORTC, LED_PORTC0, GPIO_PIN_TOGGLE);
			sprintf((char *)UartStruct0.rxStr, "SW0 PUSH : 0x%02x\r\n", swState);
			UART_puts(UART0, UartStruct0.rxStr);
			UART_puts(UART1, UartStruct0.rxStr);
			DotNumState = 0;
			DotNumValue = 9;
			DotTestRowValue = ROW0;
			
		break;
		case 0x02 :		//0b 0000 0010
			GPIO_WritePin(&LED_PORTC, LED_PORTC1, GPIO_PIN_TOGGLE);
			sprintf((char *)UartStruct0.rxStr, "SW1 PUSH : 0x%02x\r\n", swState);
			UART_puts(UART0, UartStruct0.rxStr);
			UART_puts(UART1, UartStruct0.rxStr);
			DotNumState = 0;
			DotNumValue = 9;
			DotTestRowValue = ROW1;
		break;
		case 0x04 :		//0b 0000 0100
			GPIO_WritePin(&LED_PORTC, LED_PORTC2, GPIO_PIN_TOGGLE);
			sprintf((char *)UartStruct0.rxStr, "SW2 PUSH : 0x%02x\r\n", swState);
			UART_puts(UART0, UartStruct0.rxStr);
			UART_puts(UART1, UartStruct0.rxStr);
			DotNumState = 0;
			DotNumValue = 9;
			DotTestRowValue = ROW2;
		break;
		case 0x08 :		//0b 0000 1000
			GPIO_WritePin(&LED_PORTC, LED_PORTC3, GPIO_PIN_TOGGLE);
			sprintf((char *)UartStruct0.rxStr, "SW3 PUSH : 0x%02x\r\n", swState);
			UART_puts(UART0, UartStruct0.rxStr);
			UART_puts(UART1, UartStruct0.rxStr);
			DotNumState = 0;
			DotNumValue = 9;
			DotTestRowValue = ROW3;
		break;
		case 0x10 :		//0b 0001 0000
			GPIO_WritePin(&LED_PORTC, LED_PORTC4, GPIO_PIN_TOGGLE);
			sprintf((char *)UartStruct0.rxStr, "SW4 PUSH : 0x%02x\r\n", swState);
			UART_puts(UART0, UartStruct0.rxStr);
			UART_puts(UART1, UartStruct0.rxStr);
			DotNumState = 0;
			DotNumValue = 9;
			DotTestRowValue = ROW4;
		break;
		case 0x20 :		//0b 0010 0000
			GPIO_WritePin(&LED_PORTC, LED_PORTC5, GPIO_PIN_TOGGLE);
			sprintf((char *)UartStruct0.rxStr, "SW5 PUSH : 0x%02x\r\n", swState);
			UART_puts(UART0, UartStruct0.rxStr);
			UART_puts(UART1, UartStruct0.rxStr);
			DotNumState = 0;
			DotNumValue = 9;
			DotTestRowValue = ROW5;
		break;
		case 0x40 :		//0b 0100 0000
			GPIO_WritePin(&LED_PORTC, LED_PORTC6, GPIO_PIN_TOGGLE);
			sprintf((char *)UartStruct0.rxStr, "SW6 PUSH : 0x%02x\r\n", swState);
			UART_puts(UART0, UartStruct0.rxStr);
			UART_puts(UART1, UartStruct0.rxStr);
			DotNumState = 0;
			DotNumValue = 9;
			DotTestRowValue = ROW6;
		break;
		case 0x80 :		//0b 1000 0000
			GPIO_WritePin(&LED_PORTC, LED_PORTC7, GPIO_PIN_TOGGLE);
			sprintf((char *)UartStruct0.rxStr, "SW7 PUSH : 0x%02x\r\n", swState);
			UART_puts(UART0, UartStruct0.rxStr);
			UART_puts(UART1, UartStruct0.rxStr);
			DotNumState = 1;
			if(DotNumValue >= 9){
				DotNumValue = 0;
			}
			else{
				DotNumValue++;
			}
			
			
		break;
	}
}

void SW_LONG_Press_Event()
{
	/* function input START */
	
	/* funciton input END */
}