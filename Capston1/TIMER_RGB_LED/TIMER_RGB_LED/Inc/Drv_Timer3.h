/*
 * Drv_Timer.h
 *
 * Created: 2019-09-06 오전 11:10:11
 * Author : Hybus Technical Education & Design
 */ 


#ifndef DRV_TIMER3_H_
#define DRV_TIMER3_H_

#define PRESCALE_STOP			0
#define PRESCALE_1_DIV			1
#define PRESCALE_8_DIV			2
#define PRESCALE_64_DIV			3
#define PRESCALE_256_DIV		4
#define PRESCALE_1024_DIV		5

#define T3_MAX_DUTY_VAL		255

#define RGB_DDRE			DDRE
#define RGB_PORTE			PORTE
#define RGB_R_PORTE3		PE3
#define RGB_G_PORTE4		PE4
#define RGB_B_PORTE5		PE5

uint8_t T3_Prescale;

void PCPWM3_Init(unsigned char prescale);

void SetDuty3(int8_t ch, uint8_t Duty);

void Timer3_Stop();		// 타이머 정지

void Timer3_Pause();		// 타이머 일시정지

void Timer3_Resume();	// 타이머 재개

#endif /* DRV_TIMER3_H_ */