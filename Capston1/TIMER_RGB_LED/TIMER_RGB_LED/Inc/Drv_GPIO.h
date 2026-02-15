/*
 * Drv_GPIO.h
 *
 * Created: 2019-11-25 오후 3:01:00
 * Author : Hybus Technical Education & Design 
 */ 


#ifndef DRV_GPIO_H_
#define DRV_GPIO_H_


#define GPIO_PIN_RESET			0
#define GPIO_PIN_SET			1
#define GPIO_PIN_TOGGLE			-1


void GPIO_WritePin(volatile uint8_t *PORTx, uint8_t GPIO_Pin, int8_t PinState);
void GPIO_TogglePin(volatile uint8_t *PORTx, uint8_t GPIO_Pin);
void GPIO_SetPin(volatile uint8_t *PORTx, uint8_t GPIO_Pin);
void GPIO_ResetPin(volatile uint8_t *PORTx, uint8_t GPIO_Pin);
void Protect_IO_Init();

#endif /* DRV_GPIO_H_ */