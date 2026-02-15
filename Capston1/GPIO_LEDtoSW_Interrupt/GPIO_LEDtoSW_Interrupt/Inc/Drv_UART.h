/*
 * Drv_UART.h
 *
 * Created: 2019-08-30 오후 5:40:12
 * Author : Hybus Technical Education & Design
 */ 


#ifndef DRV_UART_H_
#define DRV_UART_H_

#define UART0	0
#define UART1	1
#define UART2	2

#define PACKET_START		1
#define PACKET_END			2
#define CR_LF_ENABLE		1
#define CR_LF_DISENABLE		0

#define RX_INTERRUPT_DISABLE	0
#define RX_INTERRUPT_ENABLE		1

typedef struct
{
	uint8_t txStr[128];
	uint8_t rxStr[128];
	uint8_t rxByte;
	uint8_t rxFlag;
	uint8_t CellCnt;
	uint8_t CR_LF_State;
	
	uint8_t	PacketBit;
	uint8_t	PacketBuff[256];
}UART_GENERAL_TypeDef;

extern UART_GENERAL_TypeDef UartStruct0;
extern UART_GENERAL_TypeDef UartStruct1;

void UART_Init(uint8_t UARTx, uint8_t rxInterruptFlag);
char UART_getch(uint8_t UARTx);
void UART_put(uint8_t UARTx, uint8_t data);
void UART_puts(uint8_t UARTx, uint8_t *str);

ISR(USART0_RX_vect);
ISR(USART1_RX_vect);
ISR(USART2_RX_vect);

#endif /* DRV_UART_H_ */