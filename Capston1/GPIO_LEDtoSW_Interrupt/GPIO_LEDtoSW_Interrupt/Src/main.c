/*
 * GPIO_LEDtoSW_Interrupt.c
 *
 * Created: 2019-11-26 오후 3:43:55
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
#include "Drv_Timer0.h"
#include "Drv_UART.h"
#include "Drv_GPIO.h"
#include "App_DotMatrix.h"
/* USER CODE END Includes */


/****************************************************************************************************
	<프로젝트 기능>

	SW0 ~ SW7 클릭 : UART를 통해 스위치의 Hex 정보 전달 / DIP LED 및 Dot Matrix 점멸
****************************************************************************************************/


int main(void)
{
	//Motor(Servo, Step, DC) 플로팅 현상 방지를 위한 IO 설정
	Protect_IO_Init();
	
	//LED & SW DDR,PORT Setting
	LEDtoSW_Init();
	
	//DotMatrix DDR & PORT Init & Clear
	DotMatrix_IO_Init();	
	WrDotMatrixClear();
	
	//Timer0 OCR0B CTC 2500us Setting
	Timer0_Init(14745600, TIM0_Prescaler_256, 2500, OCR0B_CTC_Select);
	
	UART_Init(UART0, RX_INTERRUPT_ENABLE);	//MCU Board에 있는 USB to Serial
	UART_Init(UART1, RX_INTERRUPT_ENABLE);	//Base Board에 있는 USB to Serial
	
	memset(&UartStruct0, 0, sizeof(UART_GENERAL_TypeDef));		//UART struct variable Init
	memset(&UartStruct1, 0, sizeof(UART_GENERAL_TypeDef));		//UART struct variable Init
	
	//LED 순차 점멸 함수
	for(int PinIndex = 0; PinIndex < 8; PinIndex++){
		GPIO_WritePin(&LED_PORTC, PinIndex, GPIO_PIN_TOGGLE);
		delay_ms(100);
	}
	for(int PinIndex = 0; PinIndex < 8; PinIndex++){
		GPIO_WritePin(&LED_PORTC, PinIndex, GPIO_PIN_TOGGLE);
		delay_ms(100);
	}
	
	UART_puts(UART0, (uint8_t*)"Hello Multi-MCU\r\n");
	UART_puts(UART1, (uint8_t*)"Hello Multi-MCU\r\n");
	delay_ms(200);
	UART_puts(UART0, (uint8_t*)"MCU Board UART0\r\n");
	UART_puts(UART1, (uint8_t*)"Base Board UART1\r\n");
	delay_ms(200);
	
	sei();
	
	
	
    /* Replace with your application code */
    while (1) 
    {
		//메인 사이클 마다 스위치 상태 읽기 함수
		SW_READ(&hSwtich);
		
		
		if(DotNumState == 0){
			//도트메트릭스 응용 함수(스위치 클릭에 따른 이벤트 발생)
			WrDotMatrixRowArry(DotTestRowValue);
		}
		else if(DotNumState == 1){
			WrDotMatrixNumDisPlay(DotNumValue);
		}
		
		
    }
}

