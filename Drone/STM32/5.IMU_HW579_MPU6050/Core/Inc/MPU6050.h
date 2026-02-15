/*
 * GY521.h
 *
 *  Created on: 2022. 5. 9.
 *      Author: user
 */

#include "MPU6050REG.h"

#ifndef __MAIN_H
#include"main.h"
#endif

#ifndef  _MATH_H_
#include<math.h>
#endif

#ifndef INC_I2C_H_
#include "I2C.h"
#endif

#include "stdio.h"

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#define MPU6050_ADDRESS 0b1101000<<1 // AD0pin = 0
#define alpha 0.9997
#define dt 0.001

typedef struct __MPU6050{
	I2C_struct I2C;

	uint8_t gyro_address;

}MPU6050;

typedef struct{
	int32_t getmpuaccx,getmpuaccy,getmpuaccz;

	float pitch, roll;
	float f_gyx, f_gyy,f_gyz;

	float pitch_offset, roll_offset;		//pitch, roll
	int16_t gyro_cal[3];				//x,y,z

} ANGLE;

void MPU6050_Gyro_Writebyte(MPU6050 * mpu6050, uint8_t register_address, uint8_t data);

uint8_t MPU6050_Gyro_Readbyte(MPU6050 * mpu6050,uint8_t register_address);


void MPU_Reset(MPU6050* mpu6050);


void MPU6050_Init(MPU6050* mpu6050, I2C_TypeDef* i2c);

void MPU_Readaccgyro(MPU6050* mpu6050,ANGLE* angle);


void MPU_Gyrocali(MPU6050* mpu6050,ANGLE* angle);

void MPU_Angcali(MPU6050* mpu6050,ANGLE* angle);




#endif /* INC_MPU6050_H_ */
