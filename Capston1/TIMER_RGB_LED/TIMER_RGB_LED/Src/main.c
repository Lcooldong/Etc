/*
 * TIMER_RGB_LED.c
 *
 * Created: 2019-11-28 오전 11:13:39
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
#include "Drv_GPIO.h"
#include "Drv_Timer3.h"
/* USER CODE END Includes */


int main(void)
{
	//Motor(Servo, Step, DC) 플로팅 현상 방지를 위한 IO 설정
	Protect_IO_Init();
	
	int index = 0;
	
	//Timer3 RGB LED PWM Init
	PCPWM3_Init(PRESCALE_1_DIV);
	
	sei();
	
	
	for(index=0; index<=100; index++){
		SetDuty3(RGB_R_PORTE3, index);
		delay_ms(20);
	}
	SetDuty3(RGB_R_PORTE3, 0);
	for(index=0; index<=100; index++){
		SetDuty3(RGB_G_PORTE4, index);
		delay_ms(20);
	}
	SetDuty3(RGB_G_PORTE4, 0);
	for(index=0; index<=100; index++){
		SetDuty3(RGB_B_PORTE5, index);
		delay_ms(20);
	}
	SetDuty3(RGB_B_PORTE5, 0);
	
	for(index=0; index<=100; index++){
		SetDuty3(RGB_R_PORTE3, index);
		SetDuty3(RGB_G_PORTE4, index);
		SetDuty3(RGB_B_PORTE5, index);
		delay_ms(20);
	}
	
	//타이머 일시 정지
	Timer3_Pause();
	
	delay_ms(2000);
	
	//타이머 재개
	Timer3_Resume();
	
	SetDuty3(RGB_R_PORTE3, 20);
	SetDuty3(RGB_G_PORTE4, 20);
	SetDuty3(RGB_B_PORTE5, 20);
	
	
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

