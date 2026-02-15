/*
 * Drv_GPIO.c
 *
 * Created: 2019-11-25 오후 2:58:43
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
#include "Drv_GPIO.h"
/* USER CODE END Includes */


void GPIO_WritePin(volatile uint8_t *PORTx, uint8_t GPIO_Pin, int8_t PinState)
{
	if(PinState == GPIO_PIN_TOGGLE){
		*PORTx ^= (1 << GPIO_Pin);
	}
	else if(PinState == GPIO_PIN_RESET){
		*PORTx &= ~((1 << GPIO_Pin));
	}
	else if(PinState >= GPIO_PIN_SET){
		*PORTx |= (1 << GPIO_Pin);
	}
}


void GPIO_TogglePin(volatile uint8_t *PORTx, uint8_t GPIO_Pin)
{
	*PORTx ^= (1 << GPIO_Pin);
}

void GPIO_SetPin(volatile uint8_t *PORTx, uint8_t GPIO_Pin)
{
	*PORTx |= (1 << GPIO_Pin);
}

void GPIO_ResetPin(volatile uint8_t *PORTx, uint8_t GPIO_Pin)
{
	*PORTx &= ~((1 << GPIO_Pin));
}

void Protect_IO_Init()
{
	DDRH |= 0x7C;
	PORTH &= ~0x7C;
	
	DDRG |= 0x08;
	PORTG &= ~0x08;
}