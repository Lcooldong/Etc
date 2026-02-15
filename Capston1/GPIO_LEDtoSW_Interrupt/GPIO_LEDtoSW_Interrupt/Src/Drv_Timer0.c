/*
 * Drv_Timer0.c
 *
 * Created: 2019-07-15 오후 5:49:13
 * Author : Hybus Technical Education & Design 
 */ 

#define F_CPU	16000000UL

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
/* USER CODE END Includes */

/* USER CODE BEGIN Variable */
uint16_t time0_1ms_Tick = 0;
/* USER CODE END Variable */


//Prescaler(N) : 1, 8, 64, 256, 1024
void Timer0_Init(uint32_t Sys_Clk, uint16_t Prescaler, uint16_t usTime, uint8_t OCR_Select)
{
	uint8_t OCR_Value = 0;
	uint8_t CS_Value = 0;
	
	switch(Prescaler)
	{
		case TIM0_Prescaler_1 : 
			CS_Value = TIM0_Prescaler_1;
		break;
		case TIM0_Prescaler_8 :
			CS_Value = TIM0_Prescaler_8;
		break;
		case TIM0_Prescaler_64 :
			CS_Value = TIM0_Prescaler_64;
		break;
		case TIM0_Prescaler_256 :
			CS_Value = TIM0_Prescaler_256;
		break;
		case TIM0_Prescaler_1024 :
			CS_Value = TIM0_Prescaler_1024;
		break;
	}
	
	//Prescaler와 타이머 인터럽트 시간을 매개변수에 적용하여 OCR값을 결정하기 위한 수식
	//	CTC Mode Time_Clk 공식
	//	Time_Clk = Sys_Clk / ( 2 x Prescaler x (OCR + 1) )
	//	Time_Clk = 1 / Time = 1 / ( usTime / (1000 * 1000) ) = Sys_Clk / ( 2 x Prescaler x (OCR + 1) )
	
	OCR_Value = (uint8_t)( (Sys_Clk * usTime / 1000) / (2000 * Prescaler) ) - 1;
	
	//초기화
	TCCR0A = 0x00;
	TCCR0B = 0x00;
	
	//CTC 모드 설정 및 Compare Match의 기준이 되는 OC를 토글
	TCCR0A	= (0<<COM0A1) | (1<<COM0A0) | (0<<COM0B1) | (1<<COM0B0) | (1<<WGM01) | (0<<WGM00);
	TCCR0B	= (0<<FOC0A) | (0<<FOC0B) | (0<<WGM02) | (CS_Value & 0x7);
	TCNT0	= 0;
	
	if(OCR_Select == OCR0A_CTC_Select){
			
		TIMSK0	= (0<<OCIE0B) | (1<<OCIE0A) | (0<<TOIE0);
		OCR0A	= OCR_Value;
	}
	else if(OCR_Select == OCR0B_CTC_Select){
			
		TIMSK0	= (1<<OCIE0B) | (0<<OCIE0A) | (0<<TOIE0);
		OCR0A	= OCR_Value;			//CTC 모드에서 OCR0B를 사용하지 않는다. 
	}
}

//1ms마다 인터럽트 발생
ISR(TIMER0_COMPB_vect)
{
	SW_TICK_FUNC(&hSwtich);
}