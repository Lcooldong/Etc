/*
 * HW579.c
 *
 *  Created on: 2022. 5. 9.
 *      Author: user
 */


#include "HW579.h"


uint16_t rawAccelX, rawAccelY, rawAccelZ, accelAccuracy;
uint16_t rawLinAccelX, rawLinAccelY, rawLinAccelZ, accelLinAccuracy;
uint16_t rawGyroX, rawGyroY, rawGyroZ, gyroAccuracy;
uint16_t rawMagX, rawMagY, rawMagZ, magAccuracy;

HMC5883L hmc5883l;
ADXL345 adxl345;
ITG3205 itg3205;




void HW579_Init(HW579* hw579, I2C_TypeDef* i2c, double gauss)
{
	hw579->I2C.I2C = i2c;
	hw579->MAGNETO = &hmc5883l;
	hw579->ACCEL   = &adxl345;
	hw579->GYRO    = &itg3205;

	getI2C_Address(hw579);
	LL_mDelay(100);

	Magneto_Init((hw579->MAGNETO), i2c, gauss);
	Accel_Init((hw579->ACCEL), i2c);
	Gyro_Init((hw579->GYRO), i2c);


}


void getI2C_Address(HW579* hw579)
{
	uint8_t slave_address;
	uint8_t numSensors = 0;
	uint8_t sensors[3] = {0, };


	hw579->MAGNETO->I2C = hw579->I2C;
	hw579->ACCEL->I2C   = hw579->I2C;
	hw579->GYRO->I2C    = hw579->I2C;

	int state=0;
	for(slave_address = 1; slave_address < 128; slave_address++)
	{
		hw579->I2C.I2C->CR1 |= I2C_CR1_START;
		while(!(hw579->I2C.I2C->SR1 & I2C_SR1_SB));

		hw579->I2C.I2C->DR=(slave_address<<1|0);
		while(!(hw579->I2C.I2C->SR1)|!(hw579->I2C.I2C->SR2)){};
		hw579->I2C.I2C->CR1 |= I2C_CR1_STOP;

		LL_mDelay(10);	//minium wait time is 40 uS, but for sure, leave it 100 uS

		state=(hw579->I2C.I2C->SR1 & I2C_SR1_ADDR);
		if (state==2)	// get NACK after send
		{
			printf("Found I2C device at adress 0x%X (hexadecimal), or %3d (decimal) [%d]\r\n",slave_address, slave_address, numSensors);
			sensors[numSensors] = slave_address;
			numSensors++;
		}

#ifdef DEBUG_PRINT
		  printf("Address : 0x%X\r\n", slave_address);	// !HAL_OK = 1
#endif


	}


	hw579->MAGNETO->compass_address = sensors[0] << 1;
	hw579->ACCEL->accel_address	    = sensors[1] << 1;
	hw579->GYRO->gyro_address   	= sensors[2] << 1;


	printf("MAGNETO : 0x%X  = 0x%X\r\n", hw579->MAGNETO->compass_address, sensors[0]<< 1);
	printf("ACCEL   : 0x%X  = 0x%X\r\n", hw579->ACCEL->accel_address, sensors[1]<< 1);
	printf("GYRO    : 0x%X  = 0x%X\r\n", hw579->GYRO->gyro_address, sensors[2]<< 1);


#ifdef DEBUG_PRINT
	printf("MAGNETO : 0x%X\r\n", sensors[0] << 1);
	printf("ACCEL   : 0x%X\r\n", sensors[1] << 1);
	printf("GYRO    : 0x%X\r\n", sensors[2] << 1);
#endif


}
