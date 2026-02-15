/*
 * Drv_UART.c
 *
 * Created: 2019-08-30 오후 5:39:49
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
 #include "main.h"
 #include "Drv_UART.h"
 /* USER CODE END Includes */
 
 /* USER CODE BEGIN Variable */
 UART_GENERAL_TypeDef UartStruct0;
 UART_GENERAL_TypeDef UartStruct1;
 /* USER CODE END Variable */
 
 
void UART_Init(uint8_t UARTx, uint8_t rxInterruptFlag)
{
	if(UARTx == UART0){
		// 1배속 전송모드
		UCSR0A = 0x00;
		
		// RXENx=1(수신 허가),  TXENx= 1(송신 허가)
		UCSR0B = (1<<RXEN0) | (1 <<TXEN0);
		
		// RXCIEx=1(수신 인터럽트 허가)
		if(rxInterruptFlag == RX_INTERRUPT_ENABLE){
			UCSR0B |= (1<<RXCIE0);
		}
		
		// 8비트 데이터
		UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
		
		// 비동기 모드의 선택
		UCSR0C &= ~(3<<UMSEL00);
		
		//BAUD = f/16/(UBRR+1)		//UBRR = (f/16/BAUD)-1
		UBRR0H = 0x00; //보오레이트 H
		UBRR0L = 0x07; //보오레이트 L		//115200
	}
	else if(UARTx == UART1){
		// 1배속 전송모드
		UCSR1A = 0x00;
		
		// RXENx=1(수신 허가),  TXENx= 1(송신 허가)
		UCSR1B = (1<<RXEN1) | (1 <<TXEN1);
		
		// RXCIEx=1(수신 인터럽트 허가)
		if(rxInterruptFlag == RX_INTERRUPT_ENABLE){
			UCSR1B |= (1<<RXCIE1);
		}
		
		//8비트 데이터
		UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);
		
		// 비동기 모드의 선택
		UCSR1C &= ~(3<<UMSEL10);
		
		//BAUD = f/16/(UBRR+1)		//UBRR = (f/16/BAUD)-1
		UBRR1H = 0x00; //보오레이트 H
		UBRR1L = 0x07; //보오레이트 L		//115200
	}
	else if(UARTx == UART2){
		// 1배속 전송모드
		UCSR2A = 0x00; 
		// RXENx=1(수신 허가),  TXENx= 1(송신 허가)
		UCSR2B = (1<<RXEN2) | (1<<TXEN2);
		
		// RXCIEx=1(수신 인터럽트 허가)
		if(rxInterruptFlag == RX_INTERRUPT_ENABLE){
			UCSR2B |= (1<<RXCIE2);
		}
		
		//8비트 데이터
		UCSR2C = (1<<UCSZ21) | (1<<UCSZ20);
		// 비동기 모드의 선택
		UCSR2C &= ~(3<<UMSEL20); 
		
		//BAUD = f/16/(UBRR+1)		//UBRR = (f/16/BAUD)-1
		UBRR2H = 0x00; //보오레이트 H
		UBRR2L = 0x07; //보오레이트 L		//115200
	}
}

char UART_getch(uint8_t UARTx)
{
	if(UARTx == UART0){
		while(!(UCSR0A & (1<<RXC0)));
		return UDR0;
	}
	else if(UARTx == UART1){
		while(!(UCSR1A & (1<<RXC1)));
		return UDR1;
	}
	else if(UARTx == UART2){
		while(!(UCSR2A & (1<<RXC2)));
		return UDR2;
	}
	
	return 0;
}

void UART_put(uint8_t UARTx, uint8_t data)
{
	if(UARTx == UART0){
		while(!(UCSR0A & (1<<UDRE0)));	// 전송 인터럽트가 걸릴 때까지 while문 반복
		UDR0 = data;					// 데이터 값을 UDR1에 넣는다. = 전송
	}
	else if(UARTx == UART1){
		while(!(UCSR1A & (1<<UDRE1)));	// 전송 인터럽트가 걸릴 때까지 while문 반복
		UDR1 = data;					// 데이터 값을 UDR1에 넣는다. = 전송
	}
	else if(UARTx == UART2){
		while(!(UCSR2A & (1<<UDRE2)));	// 전송 인터럽트가 걸릴 때까지 while문 반복
		UDR2 = data;					// 데이터 값을 UDR1에 넣는다. = 전송
	}
}

void UART_puts(uint8_t UARTx, uint8_t *str)	// 문자열 출력 루틴
{											
	while(*str != '\0'){		// '\0'은 NULL을 의미한다.			
		UART_put(UARTx, *str);	// 문자의 마지막에는 NULL이 들어가 있으므로
		str++;					// 아스키 코드 상 ‘0’이 나올 때까지 출력한다.
	}
}

ISR(USART0_RX_vect)
{
	//Uart 데이터 레지스터에 저장된 값을 rxByte 저장
	UartStruct0.rxByte = UDR0;		
	
	//UART0에서 수신한 데이터를 UART1로 송신
	UART_put(UART1, UartStruct0.rxByte);
}

ISR(USART1_RX_vect)
{
	//Uart 데이터 레지스터에 저장된 값을 rxByte 저장
	UartStruct1.rxByte = UDR1;
	
	//UART1에서 수신한 데이터를 UART0로 송신
	UART_put(UART0, UartStruct1.rxByte);
}

ISR(USART2_RX_vect)
{
	
}