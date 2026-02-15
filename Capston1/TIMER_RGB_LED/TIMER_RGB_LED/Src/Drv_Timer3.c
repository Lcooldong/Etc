/*
 * Drv_Timer.c
 *
 * Created: 2019-09-06 오전 11:09:57
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
#include "Drv_Timer3.h"
/* USER CODE END Includes */
 
/* USER CODE BEGIN Variable */
uint8_t T3_Prescale = 0;
/* USER CODE END Variable */

void PCPWM3_Init(unsigned char prescale)
{
	RGB_DDRE = (1<<RGB_R_PORTE3) | (1<<RGB_G_PORTE4) | (1<<RGB_B_PORTE5);
	//DDRE = 0x38;	//Timer3 PWM 출력을 위한 GPIO 설정
					//TIM3A : PE3	//TIM3B : PE4	//TIM3C : PE5
	
	
/****************************************************************************************************
	PC PWM	: Phase Correct PWM Mode
	T_Hz	: 타이머 주파수
	Sys_Hz	: 시스템 주파수(16MHz)
	N		: 프리스케일러(분주비)
	TOP		: TCNT 최대값(8bit 이므로 255를 의미함)
	
	T_Hz = Sys_Hz / ( 2 x N x (1 + TOP) )
	
	* 모터 컨트롤 시 타이머 주파수는 가청 주파수 이상으로 설정하는것이 좋다.
	* 가청 주파수 :  20Hz ~ 20kHz 

****************************************************************************************************/

	// Set, Clear Time 설정
	TCCR3A &= ~(3<<COM3A0);
	TCCR3A &= ~(3<<COM3B0);
	TCCR3A &= ~(3<<COM3C0);
	
	//컴페어 매치 설정
	TCCR3A |= (2<<COM3A0);  //	Clear OCnA/OCnB/OCnC on compare match when up-counting. 
	TCCR3A |= (2<<COM3B0);	//	SetOCnA/OCnB/OCnC on compare match when down-counting.
	TCCR3A |= (2<<COM3C0);
	
	// 파형 모드 설정
	// PCPWM  WGM3     WGM0 // MAX:255
	// PCPWM : 0  0  0  1 => 8-bit	
	TCCR3A &= ~(3<<WGM30);
	TCCR3B &= ~(3<<WGM32);
	
	TCCR3A |= (0<<WGM31)|(1<<WGM30);
	TCCR3B |= (0<<WGM33)|(0<<WGM32);
	
	//프리스케일러 설정
	TCCR3B &= ~(7<<0);
	TCCR3B |= (prescale&0x7);
	
	//TCNT 및 OCR 값 초기화
	TCNT3L = 0;     TCNT3H = 0;
	
	OCR3AL  = 0;    OCR3AH  = 0;
	OCR3BL  = 0;    OCR3BH  = 0;
	OCR3CL  = 0;    OCR3CH  = 0;
	
	//인터럽트 Handler 함수 사용하지 않기 위한 설정
	TIMSK3   &= ~((1<<OCIE3A) | (1<<OCIE3B) | (1<<OCIE3C) | (1<<TOIE3));
}

void SetDuty3(int8_t ch, uint8_t Duty)
{
	uint16_t OCR_DATA;
	
	//듀티비를 100% 개념으로 스케일 조정 하기 위한 수식
	if(Duty > 100){
		Duty = 100;
	}
	OCR_DATA = ((float)Duty / 100*T3_MAX_DUTY_VAL);
	//OCR_DATA = ((float)Duty * 2.55);
	
	switch(ch)
	{
		case 0 :
		case 'A' :
		case RGB_R_PORTE3 :
			OCR3AH =  (OCR_DATA>>8)&0xff;
			OCR3AL =  (OCR_DATA&0xff);
		break;
		
		case 1 :
		case 'B' :
		case RGB_G_PORTE4 :
			OCR3BH =  (OCR_DATA>>8)&0xff;
			OCR3BL =(OCR_DATA&0xff);
		break;
		
		case 2 :
		case 'C' :
		case RGB_B_PORTE5 :
			OCR3CH =  (OCR_DATA>>8)&0xff;
			OCR3CL = (OCR_DATA&0xff);
		break;
	}
}
void Timer3_Stop()		// 타이머 정지
{								
	TCCR3B &= ~(0x07);		
}	

void Timer3_Pause()		// 타이머 일시정지
{	
	T3_Prescale = TCCR3A;	
	TCCR3A  = 0;
}	

void Timer3_Resume()	// 타이머 재개
{							
	TCCR3A  = T3_Prescale;	
}	