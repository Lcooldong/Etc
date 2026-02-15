/*
 * Drv_Timer0.h
 *
 * Created: 2019-07-15 오후 5:49:26
 * Author : Hybus Technical Education & Design
 */ 


#ifndef DRV_TIMER0_H_
#define DRV_TIMER0_H_

#define TIM0_Prescaler_1		1
#define TIM0_Prescaler_8		2
#define TIM0_Prescaler_64		3
#define TIM0_Prescaler_256		4
#define TIM0_Prescaler_1024		5

#define OCR0A_CTC_Select		0
#define OCR0B_CTC_Select		1

/* USER CODE BEGIN Variable */
extern uint16_t time0_1ms_Tick;
/* USER CODE END Variable */

void Timer0_Init(uint32_t Sys_Clk, uint16_t Prescaler, uint16_t usTime, uint8_t OCR_Select);

ISR(TIMER0_COMPB_vect);

#endif /* DRV_TIMER0_H_ */