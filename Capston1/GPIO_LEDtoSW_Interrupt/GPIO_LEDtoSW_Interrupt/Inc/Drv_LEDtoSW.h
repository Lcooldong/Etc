/*
 * Drv_LEDtoSW.c
 *
 * Created: 2019-07-15 오후 2:12:37
 * Author : Hybus Technical Education & Design 
 */ 

#ifndef	Drv_LEDtoSW_H_
#define Drv_LEDtoSW_H_

#define LED_DDRC		DDRC
#define LED_PORTC		PORTC
#define LED_PORTC0		PC0
#define LED_PORTC1		PC1
#define LED_PORTC2		PC2
#define LED_PORTC3		PC3
#define LED_PORTC4		PC4
#define LED_PORTC5		PC5
#define LED_PORTC6		PC6
#define LED_PORTC7		PC7

#define MCU_RGB_LED_G	PC0
#define MCU_RGB_LED_R	PC1
#define MCU_RGB_LED_B	PC2

#define SW_DDRL			DDRL
#define SW_PINL			PINL
#define SW_PINL0		PINL0
#define SW_PINL1		PINL1
#define SW_PINL2		PINL2
#define SW_PINL3		PINL3
#define SW_PINL4		PINL4
#define SW_PINL5		PINL5
#define SW_PINL6		PINL6
#define SW_PINL7		PINL7

#define LONGPRESS	800		// 800 x 2.5ms = 2000ms
#define SHORTPRESS	20		// 20 x 2.5ms = 50ms



typedef struct
{
	uint16_t	Delaycount;		//키 눌린 시간 카운트
	uint8_t		Longpress;		//키 길게 눌림
	uint8_t		Pressed;		//키 처음 눌림
	uint8_t		State;			//키 상태
	uint8_t		Value;			//키 값
}SWITCH_TypeDef;


extern SWITCH_TypeDef hSwtich;



void LEDtoSW_Init();

uint8_t SW_StatusPin();

void SW_READ(SWITCH_TypeDef *hSW);

void SW_TICK_FUNC(SWITCH_TypeDef *hSW);

void SW_SHORT_Press_Event(uint8_t swState);

void SW_LONG_Press_Event();

#endif