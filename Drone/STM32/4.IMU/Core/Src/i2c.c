/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**I2C1 GPIO Configuration
  PB8   ------> I2C1_SCL
  PB9   ------> I2C1_SDA
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */

  /** I2C Initialization
  */
  LL_I2C_DisableOwnAddress2(I2C1);
  LL_I2C_DisableGeneralCall(I2C1);
  LL_I2C_EnableClockStretching(I2C1);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.ClockSpeed = 100000;
  I2C_InitStruct.DutyCycle = LL_I2C_DUTYCYCLE_2;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 0;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C1, &I2C_InitStruct);
  LL_I2C_SetOwnAddress2(I2C1, 0);
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}
/* I2C2 init function */
void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**I2C2 GPIO Configuration
  PB10   ------> I2C2_SCL
  PB11   ------> I2C2_SDA
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10|LL_GPIO_PIN_11;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */

  /** I2C Initialization
  */
  LL_I2C_DisableOwnAddress2(I2C2);
  LL_I2C_DisableGeneralCall(I2C2);
  LL_I2C_EnableClockStretching(I2C2);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.ClockSpeed = 100000;
  I2C_InitStruct.DutyCycle = LL_I2C_DUTYCYCLE_2;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 0;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C2, &I2C_InitStruct);
  LL_I2C_SetOwnAddress2(I2C2, 0);
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/* USER CODE BEGIN 1 */
void I2C_Transmit(I2C_struct* I2C, uint8_t address, uint8_t* data, uint16_t size){
	if(I2C->i2c_busy==1) return;
	I2C->i2c_busy = 1;
	I2C->i2c_IsRx = 0;

	LL_I2C_DisableIT_TX(I2C->I2C);				// Disable Interrupt

	if(!LL_I2C_IsEnabled(I2C->I2C)){				// Enable I2C
		LL_I2C_Enable(I2C->I2C);
	}
	LL_I2C_DisableBitPOS(I2C->I2C);				// Disable bit Position

	while(LL_I2C_IsActiveFlag_BUSY(I2C->I2C));	// wait for Ready

	LL_I2C_GenerateStartCondition(I2C->I2C);		// Start bit

	while(!LL_I2C_IsActiveFlag_SB(I2C->I2C));	// wait start bit
	LL_I2C_TransmitData8(I2C->I2C, address);		// transmit address

	while(!LL_I2C_IsActiveFlag_ADDR(I2C->I2C));	// wait for transmitting address
	LL_I2C_ClearFlag_ADDR(I2C->I2C);				// clear address flag

	for(uint16_t i=0; i<size; i++){
		LL_I2C_TransmitData8(I2C->I2C, data[i]);	// transmit data array
		while(!LL_I2C_IsActiveFlag_TXE(I2C->I2C));	// wait to complete transmitting data
	}
	while(!LL_I2C_IsActiveFlag_BTF(I2C->I2C));	// wait Byte Transfer Finished flag.

	LL_I2C_GenerateStopCondition(I2C->I2C);		// Stop bit (ready last data and NACK)

	I2C->i2c_busy = 0;
}


void I2C_Receive(I2C_struct* I2C, uint8_t address, uint8_t* outputdata, uint16_t size){
	if(I2C->i2c_busy==1) return;
	I2C->i2c_busy = 1;
	I2C->i2c_IsRx = 1;


	address |= (0x01);	// LSB Set 1 for address_register_Read (Write => 0)

	LL_I2C_DisableIT_RX(I2C->I2C);							//Disable Interrupt

	if(!LL_I2C_IsEnabled(I2C->I2C)){						// Enable I2C
		LL_I2C_Enable(I2C->I2C);
	}
	LL_I2C_DisableBitPOS(I2C->I2C);							// Disable bit Position

	while(LL_I2C_IsActiveFlag_BUSY(I2C->I2C));				// wait for Ready

	if(size==1){											// when data size == 1
		LL_I2C_AcknowledgeNextData(I2C->I2C, LL_I2C_NACK);	// prepare to generate Non ACKnowledge after data receive
	}else{													// when data size > 1
		LL_I2C_AcknowledgeNextData(I2C->I2C, LL_I2C_ACK);	// ACKnowledge
	}

	LL_I2C_GenerateStartCondition(I2C->I2C);				// Start bit

	while(!LL_I2C_IsActiveFlag_SB(I2C->I2C));				// wait start bit
	LL_I2C_TransmitData8(I2C->I2C, address);				// transmit address

	while(!LL_I2C_IsActiveFlag_ADDR(I2C->I2C));				// wait for transmitting address
	LL_I2C_ClearFlag_ADDR(I2C->I2C);						// clear address flag

	for(uint16_t i=0; i<size; i++){
		if(i==(size - 1)){									// after receiving last data, prepare to generate Non ACKnowledge
			LL_I2C_AcknowledgeNextData(I2C->I2C, LL_I2C_NACK);
		}
		while(!LL_I2C_IsActiveFlag_RXNE(I2C->I2C));			// wait to complete receiving data
		outputdata[i] = LL_I2C_ReceiveData8(I2C->I2C);		// save data in array
	}
	LL_I2C_GenerateStopCondition(I2C->I2C);					// Stop bit

	I2C->i2c_busy = 0;
}












/* USER CODE END 1 */
