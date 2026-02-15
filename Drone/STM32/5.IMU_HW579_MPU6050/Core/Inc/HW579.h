/*
 * HW579.h
 *
 *  Created on: 2022. 5. 9.
 *      Author: user
 */

#ifndef __MAIN_H
#include "main.h"
#endif

#ifndef  _MATH_H_
#include<math.h>
#endif

#ifndef INC_I2C_H_
#include "I2C.h"
#endif

#ifndef INC_HMC5883L_H_
#include "HMC5883L.h"
#endif

#ifndef INC_ADXL345_H_
#include "ADXL345.h"
#endif

#ifndef INC_ITG3205_H_
#include "ITG3205.h"
#endif

#include "stdio.h"


#ifndef INC_HW579_H_
#define INC_HW579_H_

extern HMC5883L hmc5883l;
extern ADXL345 adxl345;
extern ITG3205 itg3205;

typedef struct __HW579{
	I2C_struct I2C;

	HMC5883L* MAGNETO;
	ADXL345* ACCEL;
	ITG3205* GYRO;



}HW579;

void HW579_Init(HW579* hw579, I2C_TypeDef* i2c, double gauss);
void getI2C_Address(HW579* hw579);

















#endif /* INC_HW579_H_ */
