#ifndef __CAN_SPI_H
#define __CAN_SPI_H

#include "stm32f4xx_hal.h"
#include "main.h"
//#include "stdbool.h"

typedef union{
	struct{
		uint8_t idType;
		uint32_t id;
		uint8_t dlc;
		uint8_t data0;
		uint8_t data1;
		uint8_t data2;
		uint8_t data3;
		uint8_t data4;
		uint8_t data5;
		uint8_t data6;
		uint8_t data7;
	} frame;
	uint8_t array[14];		// 14 bytes
}uCAN_MSG;


#define dSTANDARD_CAN_MSG_ID_2_0B 1
#define dEXTENDED_CAN_MSG_ID_2_0B 2

#define OSC_8MHZ	0
#define OSC_16MHZ	1


bool CANSPI_Initialize(uint8_t OSC_HZ, uint16_t kpbs);
uint8_t CANSPI_SetMaskFilter(uint8_t maskNumber, uint32_t maskID, uint8_t filterNumber, uint32_t filterID);
void CANSPI_Sleep(void);
uint8_t CANSPI_Transmit(uCAN_MSG *tempCanMsg);
uint8_t CANSPI_Receive(uCAN_MSG *tempCanMsg);
uint8_t CANSPI_messageInBuffer(void);
uint8_t CANSPI_isBusOff(void);
uint8_t CANSPI_isRxErrorPassive(void);
uint8_t CANSPI_isTxErrorPassive(void);

#endif	/* __CAN_SPI_H */
