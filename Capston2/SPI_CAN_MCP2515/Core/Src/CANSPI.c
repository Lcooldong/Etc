#include "CANSPI.h"
#include "MCP2515.h"


#include "stdio.h"
#include "usart.h"
extern UART_HandleTypeDef huart3;
extern uint8_t ReceiveDataArray[8];



/* Local Function Prototypes */
static uint32_t convertReg2ExtendedCANid(uint8_t tempRXBn_EIDH, uint8_t tempRXBn_EIDL, uint8_t tempRXBn_SIDH, uint8_t tempRXBn_SIDL);
static uint32_t convertReg2StandardCANid(uint8_t tempRXBn_SIDH, uint8_t tempRXBn_SIDL);
static void convertCANid2Reg(uint32_t tempPassedInID, uint8_t canIdType, id_reg_t *passedIdReg);

/* Local Variables */
ctrl_status_t ctrlStatus;
ctrl_error_status_t errorStatus;
id_reg_t idReg;


/* CAN SPI APIs */

/* Enter Sleep Mode */
void CANSPI_Sleep(void)
{
	/* Clear CAN bus wakeup interrupt */
	MCP2515_BitModify(MCP2515_CANINTF, 0x40, 0x00);
	
	/* Enable CAN bus activity wakeup */
	MCP2515_BitModify(MCP2515_CANINTE, 0x40, 0x40);
	
	MCP2515_SetSleepMode();
}


/* Initialize CAN Communication */
bool CANSPI_Initialize(uint8_t OSC_HZ, uint16_t kbps)
{

	RXF0 RXF0reg;
	RXF1 RXF1reg;
	RXF2 RXF2reg;
	RXF3 RXF3reg;
	RXF4 RXF4reg;
	RXF5 RXF5reg;
	RXM0 RXM0reg;
	RXM1 RXM1reg;
	
	/* Initialize Rx Mask Values */
	RXM0reg.RXM0SIDH = 0x00;
	RXM0reg.RXM0SIDL = 0x00;
	RXM0reg.RXM0EID8 = 0x00;
	RXM0reg.RXM0EID0 = 0x00;
	
	RXM1reg.RXM1SIDH = 0x00;
	RXM1reg.RXM1SIDL = 0x00;
	RXM1reg.RXM1EID8 = 0x00;
	RXM1reg.RXM1EID0 = 0x00;
	
	/* Initialize Rx Filter Values */
	RXF0reg.RXF0SIDH = 0x00;
	RXF0reg.RXF0SIDL = 0x00;		// Standard Filter
	RXF0reg.RXF0EID8 = 0x00;
	RXF0reg.RXF0EID0 = 0x00;
	
	RXF1reg.RXF1SIDH = 0x00;
	RXF1reg.RXF1SIDL = 0x00;		// Extended Filter
	RXF1reg.RXF1EID8 = 0x00;
	RXF1reg.RXF1EID0 = 0x00;
	
	RXF2reg.RXF2SIDH = 0x00;
	RXF2reg.RXF2SIDL = 0x00;		
	RXF2reg.RXF2EID8 = 0x00;
	RXF2reg.RXF2EID0 = 0x00;
	
	RXF3reg.RXF3SIDH = 0x00;
	RXF3reg.RXF3SIDL = 0x00;		
	RXF3reg.RXF3EID8 = 0x00;
	RXF3reg.RXF3EID0 = 0x00;
	
	RXF4reg.RXF4SIDH = 0x00;
	RXF4reg.RXF4SIDL = 0x00;		
	RXF4reg.RXF4EID8 = 0x00;
	RXF4reg.RXF4EID0 = 0x00;
	
	RXF5reg.RXF5SIDH = 0x00;
	RXF5reg.RXF5SIDL = 0x00;		
	RXF5reg.RXF5EID8 = 0x00;
	RXF5reg.RXF5EID0 = 0x00;
	
	/* Initialize MCP2515, Check SPI Communication Condition */
	if(!MCP2515_Initialize())
		return false;
	
	/* Set to Configuration Mode */
	if(!MCP2515_SetConfigMode())
		return false;
	
	/* Set up Filter & Mask */
	MCP2515_WriteByteSequence(MCP2515_RXM0SIDH, MCP2515_RXM0EID0, &(RXM0reg.RXM0SIDH));
	MCP2515_WriteByteSequence(MCP2515_RXM1SIDH, MCP2515_RXM1EID0, &(RXM1reg.RXM1SIDH));
	
	MCP2515_WriteByteSequence(MCP2515_RXF0SIDH, MCP2515_RXF0EID0, &(RXF0reg.RXF0SIDH));
	MCP2515_WriteByteSequence(MCP2515_RXF1SIDH, MCP2515_RXF1EID0, &(RXF1reg.RXF1SIDH));
	MCP2515_WriteByteSequence(MCP2515_RXF2SIDH, MCP2515_RXF2EID0, &(RXF2reg.RXF2SIDH));
	MCP2515_WriteByteSequence(MCP2515_RXF3SIDH, MCP2515_RXF3EID0, &(RXF3reg.RXF3SIDH));
	MCP2515_WriteByteSequence(MCP2515_RXF4SIDH, MCP2515_RXF4EID0, &(RXF4reg.RXF4SIDH));
	MCP2515_WriteByteSequence(MCP2515_RXF5SIDH, MCP2515_RXF5EID0, &(RXF5reg.RXF5SIDH));
	
	
	/* Accept All (Standard + Extended) */
	MCP2515_WriteByte(MCP2515_RXB0CTRL, 0x04);	// Enable BUKT, Accept Filter 0
	MCP2515_WriteByte(MCP2515_RXB1CTRL, 0x01);	// Accept Filter 1

	const long double tq_8MHZ  = 0.00000025;		// 0.25us
	const long double tq_16MHZ = 0.000000125;		// 0.125us
	const double KHZ = 0.001;				// 1 ms
	const	uint32_t KHZPER8MHZ	= (uint32_t)(KHZ/tq_8MHZ);			// 4000	
	const	uint32_t KHZPER16MHZ	= (uint32_t)(KHZ/tq_16MHZ);			// 8000	

	
//#ifdef OSC_16MHZ	
	if(OSC_HZ == OSC_16MHZ){
		
	/* 
  * tq = 2 * (brp(0) + 1) / 16000000 = 0.125us
  * tbit = (SYNC_SEG(1 fixed) + PROP_SEG + PS1 + PS2)
  * 16tq = 2us = 500kbps
	* tbit = 1tq + 5tq + 6tq + 4tq = 16tq
  */
	
	// minimum 5 tq, maximun 25 tq	
	if((kbps >= 320) && (kbps <= 1000))
	{
		uint8_t numberOftq	= (uint8_t)(KHZPER16MHZ/kbps);	// Number of TimeQuantum
		uint8_t samplePoint = (uint8_t)(numberOftq * 0.75);
		uint8_t PROP_tq =	(uint8_t)(samplePoint / 2);
		uint8_t PS1_tq = samplePoint - PROP_tq - 1;
		uint8_t PS2_tq = numberOftq - samplePoint;
		
		printf("numberOftq   : %d\r\nSAMPLE_POINT : %d\r\nPROP_tq      : %d\r\nPS1          : %d\r\nPS2          : %d\r\n", numberOftq, samplePoint, PROP_tq, PS1_tq, PS2_tq);
		
		uint8_t CNF2_Buffer = 0xC0;
		uint8_t CNF3_Buffer = 0x80;
		
		CNF2_Buffer |= (PS1_tq - 1) << 3;
		CNF2_Buffer |= (PROP_tq - 1);

		CNF3_Buffer |= (PS2_tq - 1);
		
		/* 00(SJW 1tq) 000000 */  
		MCP2515_WriteByte(MCP2515_CNF1, 0x00);
  
		/* 1 1 100(5tq) 101(6tq) */  
		MCP2515_WriteByte(MCP2515_CNF2, CNF2_Buffer);
  
		/* 1 0 000 011(4tq) */  
		MCP2515_WriteByte(MCP2515_CNF3, CNF3_Buffer);	
	}	
	else
	{
		/* Default 500kbps */
		
		/* 00(SJW 1tq) 000000 */  
		MCP2515_WriteByte(MCP2515_CNF1, 0x00);
  
		/* 1 1 100(5tq) 101(6tq) */  
		MCP2515_WriteByte(MCP2515_CNF2, 0xE5);
  
		/* 1 0 000 011(4tq) */  
		MCP2515_WriteByte(MCP2515_CNF3, 0x83);	
	}	
	
	}
		

		
	
//#elif defined OSC_8MHZ
	else if(OSC_HZ == OSC_8MHZ){
	/*
	@ tq   = 2 * (brp(0) + 1) / 8,000,000 = 0.25 us
	@ tbit = (SYNC_SEG(1 fixed) + PROP_SEG + PS1 + PS2)
	@ 8tq  = 2us = 500kbps
	@ tbit = 1tq + 2tq + 2tq + 3tq
	*/
	
	
	printf("%.9Lf\r\n%.3lf\r\n%d\r\n", tq_8MHZ, KHZ, KHZPER8MHZ);	
		
	// minimum 5 tq, maximun 25 tq	
	if((kbps >= 160) && (kbps <= 800))
	{
		uint8_t numberOftq	= (uint8_t)(KHZPER8MHZ/kbps);	// Number of TimeQuantum
		uint8_t samplePoint = (uint8_t)(numberOftq * 0.75);
		uint8_t PROP_tq =	(uint8_t)(samplePoint / 2);
		uint8_t PS1_tq = samplePoint - PROP_tq - 1;
		uint8_t PS2_tq = numberOftq - samplePoint;
		
		printf("numberOftq   : %d\r\nSAMPLE_POINT : %d\r\nPROP_tq      : %d\r\nPS1          : %d\r\nPS2          : %d\r\n", numberOftq, samplePoint, PROP_tq, PS1_tq, PS2_tq);
		
		uint8_t CNF2_Buffer = 0xC0;
		uint8_t CNF3_Buffer = 0x80;
		
		CNF2_Buffer |= (PS1_tq - 1) << 3;
		CNF2_Buffer |= (PROP_tq - 1);

		CNF3_Buffer |= (PS2_tq - 1);
		
		
		/* 00 (SJW 1tq) 000 000 */
		MCP2515_WriteByte(MCP2515_CNF1, 0x00);
		
		/* 1 1 PS1(3 bits) PROP(3 bits) */
		MCP2515_WriteByte(MCP2515_CNF2, CNF2_Buffer);
	
		/* 1 0 000 PS2(3 bits) */
		MCP2515_WriteByte(MCP2515_CNF3, CNF3_Buffer);
		
	}
	else
	{
		printf("out of range : default 500 kbps (reset between 160~800) \r\n");
		
		/* Default 500kbps */
		
		/* 00(SJW 1tq) 000 000	(Baud Rate Prescaler = 0) */
		MCP2515_WriteByte(MCP2515_CNF1, 0x00);
	
		/* 1 1 001(2tq) 001(2tq) */
		MCP2515_WriteByte(MCP2515_CNF2, 0xC9);
	
		/* 1 0 000 010 (3tq) */
		MCP2515_WriteByte(MCP2515_CNF3, 0x82);
	}		


//#endif
	}
	
	
	/* Set to Normal Mode */
	if(!MCP2515_SetNormalMode())
		return false;
	
	return true;
}




/* Set Filter and Mask */
uint8_t CANSPI_SetMaskFilter(uint8_t maskNumber, uint32_t maskID, uint8_t filterNumber, uint32_t filterID)
{
	RXF0 RXF0reg;
	RXF1 RXF1reg;
	RXF2 RXF2reg;
	RXF3 RXF3reg;
	RXF4 RXF4reg;
	RXF5 RXF5reg;
	RXM0 RXM0reg;
	RXM1 RXM1reg;
	
	
	if((maskNumber == 0) || (maskNumber == 1))
	{
		switch(maskNumber)
		{	
			case 0:
				//convertReg2ExtendedCANid(uint8_t tempRXBn_EIDH, uint8_t tempRXBn_EIDL, uint8_t tempRXBn_SIDH, uint8_t tempRXBn_SIDL);
				RXM0reg.RXM0SIDH = (maskID >> 3) & 0xFF;			// SID 10~3
				RXM0reg.RXM0SIDL = ((maskID & 0x07) << 5)  | ((maskID >> 27) & 0x03);		// SID 2,1,0	EID 17,16
				RXM0reg.RXM0EID8 = (maskID >> 19) & 0xFF;
				RXM0reg.RXM0EID0 = (maskID >> 11) & 0xFF;
				printf("MaskNumber : %d\r\n", maskNumber);
				printf("MaskID : 0x%x\r\n", maskID);
				printf("M0SIDH %x\r\n",RXM0reg.RXM0SIDH);
				printf("M0SIDL %x\r\n",RXM0reg.RXM0SIDL);
				printf("M0EID8 %x\r\n",RXM0reg.RXM0EID8);
				printf("M0EID0 %x\r\n",RXM0reg.RXM0EID0);
				break;
		
		
			case 1:
				RXM1reg.RXM1SIDH = (maskID >> 3) & 0xFF;			// SID 10~3
				RXM1reg.RXM1SIDL = ((maskID & 0x07) << 5)  | ((maskID >> 27) & 0x03);		// SID 2,1,0	EID 17,16
				RXM1reg.RXM1EID8 = (maskID >> 19) & 0xFF;
				RXM1reg.RXM1EID0 = (maskID >> 11) & 0xFF;
				printf("M1SIDH %x\r\n",RXM1reg.RXM1SIDH);
				printf("M1SIDL %x\r\n",RXM1reg.RXM1SIDL);
				printf("M1EID8 %x\r\n",RXM1reg.RXM1EID8);
				printf("M1EID0 %x\r\n",RXM1reg.RXM1EID0);
				break;
		}
	}
	else
	{
		printf("out of range : maskNumber\r\n");
		return false;
	}
	
	if(filterNumber <=5)
	{
		switch(filterNumber)
		{	
			case 0:
				/* Extended */
				if (filterID > 0x7FF)
				{
					RXF0reg.RXF0SIDH = (filterID >> 3) & 0xFF;			// SID 10~3
					RXF0reg.RXF0SIDL = ((filterID & 0x07) << 5) | ((maskID >> 27) & 0x03) + 0x08;		// SID 2,1,0,  (EXIDE = 1), EID 17,16
					RXF0reg.RXF0EID8 = (filterID >> 19) & 0xFF;
					RXF0reg.RXF0EID0 = (filterID >> 11) & 0xFF;
				}
				else
				{		
					/* Standard */
					//convertReg2StandardCANid(uint8_t tempRXBn_SIDH, uint8_t tempRXBn_SIDL);
					RXF0reg.RXF0SIDH = (filterID >> 3) & 0xFF;
					RXF0reg.RXF0SIDL &= (~0xFF);
					RXF0reg.RXF0SIDL = ((filterID & 0x07) << 5);
					printf("filterNumber : %d\r\n", filterNumber);
					printf("filterID : 0x%x\r\n", filterID);
					printf("F0SIDH %x\r\n",RXF0reg.RXF0SIDH);
					printf("F0SIDL %x\r\n",RXF0reg.RXF0SIDL);
					printf("F0EID8 %x\r\n",RXF0reg.RXF0EID8);
					printf("F0EID0 %x\r\n",RXF0reg.RXF0EID0);
				}		
				break;
			
			case 1:
				/* Extended */
				if (filterID > 0x7FF)
				{
					RXF1reg.RXF1SIDH = (filterID >> 3) & 0xFF;			// SID 10~3
					RXF1reg.RXF1SIDL = ((filterID & 0x07) << 5) | ((filterID >> 27) & 0x03) + 0x08;		// SID 2,1,0,  (EXIDE = 1), EID 17,16
					RXF1reg.RXF1EID8 = (filterID >> 19) & 0xFF;
					RXF1reg.RXF1EID0 = (filterID >> 11) & 0xFF;
				}
				else
				{		
					/* Standard */
					RXF1reg.RXF1SIDH = (filterID >> 3) & 0xFF;
					RXF1reg.RXF1SIDL &= (~0xFF);
					RXF1reg.RXF1SIDL = ((filterID & 0x07) << 5);
				}	
				break;
			
			case 2:
				/* Extended */
				if (filterID > 0x7FF)
				{
					RXF2reg.RXF2SIDH = (filterID >> 3) & 0xFF;			// SID 10~3
					RXF2reg.RXF2SIDL = ((filterID & 0x07) << 5) | ((filterID >> 27) & 0x03) + 0x08;		// SID 2,1,0,  (EXIDE = 1), EID 17,16
					RXF2reg.RXF2EID8 = (filterID >> 19) & 0xFF;
					RXF2reg.RXF2EID0 = (filterID >> 11) & 0xFF;
				}
				else
				{		
					/* Standard */
					RXF2reg.RXF2SIDH = (filterID >> 3) & 0xFF;
					RXF2reg.RXF2SIDL &= (~0xFF);
					RXF2reg.RXF2SIDL = ((filterID & 0x07) << 5);
				}	
				break;
			
			case 3:
				/* Extended */
				if (filterID > 0x7FF)
				{
					RXF3reg.RXF3SIDH = (filterID >> 3) & 0xFF;			// SID 10~3
					RXF3reg.RXF3SIDL = ((filterID & 0x07) << 5) | ((filterID >> 27) & 0x03) + 0x08;		// SID 2,1,0,  (EXIDE = 1), EID 17,16
					RXF3reg.RXF3EID8 = (filterID >> 19) & 0xFF;
					RXF3reg.RXF3EID0 = (filterID >> 11) & 0xFF;
				}
				else
				{		
					/* Standard */
					RXF3reg.RXF3SIDH = (filterID >> 3) & 0xFF;
					RXF3reg.RXF3SIDL &= (~0xFF);
					RXF3reg.RXF3SIDL = ((filterID & 0x07) << 5);
				}	
				break;
			
			case 4:
				/* Extended */
				if (filterID > 0x7FF)
				{
					RXF3reg.RXF3SIDH = (filterID >> 3) & 0xFF;			// SID 10~3
					RXF3reg.RXF3SIDL = ((filterID & 0x07) << 5) | ((filterID >> 27) & 0x03) + 0x08;		// SID 2,1,0,  (EXIDE = 1), EID 17,16
					RXF3reg.RXF3EID8 = (filterID >> 19) & 0xFF;
					RXF3reg.RXF3EID0 = (filterID >> 11) & 0xFF;
				}
				else
				{		
					/* Standard */
					RXF4reg.RXF4SIDH = (filterID >> 3) & 0xFF;
					RXF4reg.RXF4SIDL &= (~0xFF);
					RXF4reg.RXF4SIDL = ((filterID & 0x07) << 5);
				}	
				break;
			
			case 5:
				/* Extended */
				if (filterID > 0x7FF)
				{
					RXF5reg.RXF5SIDH = (filterID >> 3) & 0xFF;			// SID 10~3
					RXF5reg.RXF5SIDL = ((filterID & 0x07) << 5) | ((filterID >> 27) & 0x03) + 0x08;		// SID 2,1,0,  (EXIDE = 1), EID 17,16
					RXF5reg.RXF5EID8 = (filterID >> 19) & 0xFF;
					RXF5reg.RXF5EID0 = (filterID >> 11) & 0xFF;
				}
				else
				{		
					/* Standard */
					RXF5reg.RXF5SIDH = (filterID >> 3) & 0xFF;
					RXF5reg.RXF5SIDL &= (~0xFF);
					RXF5reg.RXF5SIDL = ((filterID & 0x07) << 5);
				}	
				break;
		}	
	}
	else
	{
		printf("out of range : filterNumber\r\n");
		return false;
	}
	
	
	
	
		/* Initialize MCP2515, Check SPI Communication Condition */
	if(!MCP2515_Initialize())
		return false;
	
	/* Set to Configuration Mode */
	if(!MCP2515_SetConfigMode())
		return false;
	
	/* Set up Filter & Mask */
	MCP2515_WriteByteSequence(MCP2515_RXM0SIDH, MCP2515_RXM0EID0, &(RXM0reg.RXM0SIDH));
	MCP2515_WriteByteSequence(MCP2515_RXM1SIDH, MCP2515_RXM1EID0, &(RXM1reg.RXM1SIDH));
	
	MCP2515_WriteByteSequence(MCP2515_RXF0SIDH, MCP2515_RXF0EID0, &(RXF0reg.RXF0SIDH));
	MCP2515_WriteByteSequence(MCP2515_RXF1SIDH, MCP2515_RXF1EID0, &(RXF1reg.RXF1SIDH));
	MCP2515_WriteByteSequence(MCP2515_RXF2SIDH, MCP2515_RXF2EID0, &(RXF2reg.RXF2SIDH));
	MCP2515_WriteByteSequence(MCP2515_RXF3SIDH, MCP2515_RXF3EID0, &(RXF3reg.RXF3SIDH));
	MCP2515_WriteByteSequence(MCP2515_RXF4SIDH, MCP2515_RXF4EID0, &(RXF4reg.RXF4SIDH));
	MCP2515_WriteByteSequence(MCP2515_RXF5SIDH, MCP2515_RXF5EID0, &(RXF5reg.RXF5SIDH));
	
	
	/* Accept All (Standard + Extended) */
	if (filterNumber == 0 || filterNumber == 1){
		MCP2515_WriteByte(MCP2515_RXB0CTRL, 0x04	+ filterNumber);	// Enable BUKT, Accept Filter 0
	}
	else{
		MCP2515_WriteByte(MCP2515_RXB1CTRL, filterNumber);	// Accept Filter by filterNumber
	}
		
	
	/* Set to Normal Mode */
	if(!MCP2515_SetNormalMode())
		return false;
	
	return true;
}



/* Transmit CAN Message */
uint8_t CANSPI_Transmit(uCAN_MSG *tempCanMsg)
{
	uint8_t returnValue = 0;
	
	idReg.tempSIDH = 0;
	idReg.tempSIDL = 0;
	idReg.tempEID8 = 0;
	idReg.tempEID0 = 0;
	
	ctrlStatus.ctrl_status = MCP2515_ReadStatus();
	
	/* Transmit unpending Transmission Buffer */
	if(ctrlStatus.TXB0REQ != 1){
		
		/* convert to ID Type */
		convertCANid2Reg(tempCanMsg->frame.id, tempCanMsg->frame.idType, &idReg);
		
		/* Load data to TxBuffer */
		MCP2515_LoadTxSequence(MCP2515_LOAD_TXB0SIDH, &(idReg.tempSIDH), tempCanMsg->frame.dlc, &(tempCanMsg->frame.data0));
		
		/* Request TxBuffer data */
		MCP2515_RequestToSend(MCP2515_RTS_TX0);
		
		returnValue = 1;
	}
	else if(ctrlStatus.TXB1REQ != 1)
	{
		
		/* convert to ID Type */
		convertCANid2Reg(tempCanMsg->frame.id, tempCanMsg->frame.idType, &idReg);
		
		/* Load data to TxBuffer */
		MCP2515_LoadTxSequence(MCP2515_LOAD_TXB1SIDH, &(idReg.tempSIDH), tempCanMsg->frame.dlc, &(tempCanMsg->frame.data0));
		
		/* Request TxBuffer data */
		MCP2515_RequestToSend(MCP2515_RTS_TX1);
		
		returnValue = 1;
	}
	else if(ctrlStatus.TXB2REQ != 1)
	{
		
		/* convert to ID Type */
		convertCANid2Reg(tempCanMsg->frame.id, tempCanMsg->frame.idType, &idReg);
		
		/* Load data to TxBuffer */
		MCP2515_LoadTxSequence(MCP2515_LOAD_TXB2SIDH, &(idReg.tempSIDH), tempCanMsg->frame.dlc, &(tempCanMsg->frame.data0));
		
		/* Request TxBuffer data */
		MCP2515_RequestToSend(MCP2515_RTS_TX2);
		
		returnValue = 1;
	}
	
	return (returnValue);
}



/* Receive CAN Message */
uint8_t CANSPI_Receive(uCAN_MSG *tempCanMsg)
{
	uint8_t returnValue = 0;
	rx_reg_t rxReg;
	ctrl_rx_status_t rxStatus;
	
	rxStatus.ctrl_rx_status = MCP2515_GetRxStatus();
	
	/* check received Message in rxBuffer */
	if (rxStatus.rxBuffer != 0)
	{
		/* check which buffer has Message (priority RXB0 > RXB1)*/
		if ((rxStatus.rxBuffer == MSG_IN_RXB0)|(rxStatus.rxBuffer == MSG_IN_BOTH_BUFFERS))
		{
			MCP2515_ReadRxSequence(MCP2515_READ_RXB0SIDH, rxReg.rx_reg_array, sizeof(rxReg.rx_reg_array));
		}
		else if (rxStatus.rxBuffer == MSG_IN_RXB1)
		{
			MCP2515_ReadRxSequence(MCP2515_READ_RXB1SIDH, rxReg.rx_reg_array, sizeof(rxReg.rx_reg_array));
		}
		
		
		/* Extended Type */
		if(rxStatus.msgType == dEXTENDED_CAN_MSG_ID_2_0B)
		{
			tempCanMsg->frame.idType = (uint8_t)dEXTENDED_CAN_MSG_ID_2_0B;
			tempCanMsg->frame.id		 = convertReg2ExtendedCANid(rxReg.RXBnEID8, rxReg.RXBnEID0, rxReg.RXBnSIDH, rxReg.RXBnSIDL);
		}
		else	
		{
			/* Standard Type */
			tempCanMsg->frame.idType = (uint8_t)dSTANDARD_CAN_MSG_ID_2_0B;
			tempCanMsg->frame.id		 = convertReg2StandardCANid(rxReg.RXBnSIDH, rxReg.RXBnSIDL);
		}
		
		printf("ID type  : %u\r\n", tempCanMsg->frame.idType);
		printf("RXBnSIDH : %x\r\n", rxReg.RXBnSIDH);
		printf("RXBnSIDL : %x\r\n", rxReg.RXBnSIDL);
		printf("unsigned id :%u\r\n", convertReg2StandardCANid(rxReg.RXBnSIDH, rxReg.RXBnSIDL));
		printf("id : 0x%x\r\n", convertReg2StandardCANid(rxReg.RXBnSIDH, rxReg.RXBnSIDL));
		printf("id : 0x%x\r\n", tempCanMsg->frame.id);
		
		tempCanMsg->frame.dlc		= rxReg.RXBnDLC;
		tempCanMsg->frame.data0 = rxReg.RXBnD0;
		tempCanMsg->frame.data1 = rxReg.RXBnD1;
		tempCanMsg->frame.data2 = rxReg.RXBnD2;
		tempCanMsg->frame.data3 = rxReg.RXBnD3;
		tempCanMsg->frame.data4 = rxReg.RXBnD4;
		tempCanMsg->frame.data5 = rxReg.RXBnD5;
		tempCanMsg->frame.data6 = rxReg.RXBnD6;
		tempCanMsg->frame.data7 = rxReg.RXBnD7;
		
		
		ReceiveDataArray[0] = tempCanMsg->frame.data0;
		ReceiveDataArray[1] = tempCanMsg->frame.data1;
		ReceiveDataArray[2] = tempCanMsg->frame.data2;
		ReceiveDataArray[3] = tempCanMsg->frame.data3;
		ReceiveDataArray[4] = tempCanMsg->frame.data4;
		ReceiveDataArray[5] = tempCanMsg->frame.data5;
		ReceiveDataArray[6] = tempCanMsg->frame.data6;
		ReceiveDataArray[7] = tempCanMsg->frame.data7;
		
		returnValue = 1;
	}
	
	return (returnValue);
}



/* Check how many Messages place in Buffer */
uint8_t CANSPI_messagesInBuffer(void)
{
	uint8_t messageCount = 0;
	
	ctrlStatus.ctrl_status = MCP2515_ReadStatus();
	
	if(ctrlStatus.RX0IF != 0)
	{
		messageCount++;
	}
	
	if(ctrlStatus.RX1IF != 0)
	{
		messageCount++;
	}
	
	return (messageCount);
}

/* Check CAN Bus which is Offline or not */
uint8_t CANSPI_isBusOff(void)
{
	uint8_t returnValue = 0;
	
	errorStatus.error_flag_reg = MCP2515_ReadByte(MCP2515_EFLG);
	
	if(errorStatus.TXB0 == 1)
	{
		returnValue = 1;	// Bus off (Transmit Error Count == 255 -> TXB0 = 1)
	}

	return (returnValue);
}


/* Check Rx Passive Error */
uint8_t CANSPI_isRxErrorPassive(void)
{
	uint8_t returnValue = 0;
	
	errorStatus.error_flag_reg = MCP2515_ReadByte(MCP2515_EFLG);
	
	if(errorStatus.RXEP == 1)
	{
		returnValue = 1;	//	Receiver Error Count >= 128
	}
	
	return (returnValue);
}


/* Check Tx Passive Error */
uint8_t CANSPI_isTxErrorPassive(void)
{
	uint8_t returnValue = 0;
	
	errorStatus.error_flag_reg = MCP2515_ReadByte(MCP2515_EFLG);
	
	if(errorStatus.TXEP == 1)
	{
		returnValue = 1;	//	Transmit Error Count >= 128
	}
	
	return (returnValue);
}


/* Register -> 29 bits Extended ID */
static uint32_t convertReg2ExtendedCANid(uint8_t tempRXBn_EIDH, uint8_t tempRXBn_EIDL, uint8_t tempRXBn_SIDH, uint8_t tempRXBn_SIDL)
{
	uint32_t returnValue = 0;
	uint32_t ConvertedID = 0;		// 0 ~ 31
	uint8_t CAN_standardLo_ID_lo2bits;	// standard_ID_LOW <1:0>
	uint8_t CAN_standardLo_ID_hi3bits;	// standard_ID_LOW <7:5>
	
	// |SIDH|SIDL|EID8|EID0| frame -> [31,30,29]|SIDH<28:21>|SID2, SID1, SID0|EID17, EID16|EID8<15:8>|EID0<7:0>|
	CAN_standardLo_ID_lo2bits	= (tempRXBn_SIDL & 0x03);
	CAN_standardLo_ID_hi3bits	= (tempRXBn_SIDL >> 5);				// Get 3bits	(ex:[111]0 1111 -> 0000 0[111]) SID 2,1,0
	ConvertedID = (tempRXBn_SIDH << 3);											// Get space for 3bits	(SIDH + 3 bits[0])
	ConvertedID = ConvertedID + CAN_standardLo_ID_hi3bits;	// 8 bits + 3 bits (SID 2, SID 1, SID 0)
	ConvertedID	= (ConvertedID << 2);												// Get space for 2bits
	ConvertedID	= ConvertedID	+ CAN_standardLo_ID_lo2bits;	// 11 bits + 2bits (EID17, EID16)
	ConvertedID	= (ConvertedID << 8);												// Get space for EIDH
	ConvertedID = ConvertedID + tempRXBn_EIDH;							// 13 bits + 8 bits
	ConvertedID	= (ConvertedID << 8);												// Get space for EIDL
	ConvertedID = ConvertedID + tempRXBn_EIDL;							// 21 bits + 8 bits
	returnValue = ConvertedID;															// 29 bits Extended ID
	
	return (returnValue);
}


/* Register -> 11 bits Standard ID */
static uint32_t convertReg2StandardCANid(uint8_t tempRXBn_SIDH, uint8_t tempRXBn_SIDL)
{
	uint32_t returnValue = 0;
	uint32_t ConvertedID;
	
	ConvertedID = (tempRXBn_SIDH << 3);
	//printf("%x\r\n", ConvertedID);
	
	ConvertedID = ConvertedID + (tempRXBn_SIDL >> 5);
	//printf("%x\r\n", ConvertedID);
	returnValue = ConvertedID;
	
	return (returnValue);
}

/* CAN ID -> Register */
static void convertCANid2Reg(uint32_t tempPassedInID, uint8_t canIdType, id_reg_t *passedIdReg)
{
	uint8_t wipSIDL = 0;
	
	/* Extended */
	if (canIdType == dEXTENDED_CAN_MSG_ID_2_0B)
	{
		/* EID0 */
		passedIdReg->tempEID0 = 0xFF & tempPassedInID;	// first 8 bits
		tempPassedInID = tempPassedInID >> 8;						// Go to EID8
		
		/* EID8 */
		passedIdReg->tempEID8 = 0xFF & tempPassedInID;	// second 8 bits
		tempPassedInID = tempPassedInID >> 8;						// Go to SIDL
		
		/*SIDL*/
		wipSIDL = 0x03 & tempPassedInID;								// third 2 bits, Get EID17, EID16
		tempPassedInID = tempPassedInID << 3;						// make space for SID2, SID1, SID0, (SRR), (IDE), [0, EID7, EID6]
		wipSIDL	= (0xE0 & tempPassedInID) + wipSIDL;		// SID2, SID1, SID0 + [0, 0, 0] + EID17, EID16
		wipSIDL = wipSIDL + 0x08;												// Add Extended Identifier Flag bit
		passedIdReg->tempSIDL = 0xEB & wipSIDL;					// 1110 1011
		
		/* SIDH */
		tempPassedInID = tempPassedInID >> 8;
		passedIdReg->tempSIDH = 0xFF & tempPassedInID;
	}
	else
	{
		/* Standard */
		passedIdReg->tempEID8 = 0;
		passedIdReg->tempEID0 = 0;
		tempPassedInID = tempPassedInID << 5;						// Get SID2, SID1, SID 0
		passedIdReg->tempSIDL = 0xFF & tempPassedInID;
		tempPassedInID = tempPassedInID >> 8;						// Go to SIDH
		passedIdReg->tempSIDH = 0xFF & tempPassedInID;
	
	}
}
