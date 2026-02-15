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

void LEDtoSW_Init();
void SW_SHORT_Press_Event(uint8_t swState);

#endif