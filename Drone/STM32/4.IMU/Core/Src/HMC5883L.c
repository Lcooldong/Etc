/*
 * HMC5883L.c
 *
 *  Created on: 2022. 5. 9.
 *      Author: user
 */

#ifndef INC_HMC5883L_H_
#include "HMC5883L.h"
#endif

void Magneto_Writebyte(HMC5883L* hmc5883l, uint8_t register_address, uint8_t data)
{
    uint8_t Trans[2]={register_address, data};
    I2C_Transmit(&(hmc5883l->I2C), hmc5883l->compass_address, Trans,2);
}

uint8_t Magneto_Readbyte(HMC5883L * hmc5883l, uint8_t register_address)
{
    uint8_t Trans[1]={register_address};
    uint8_t Receive[1];

    I2C_Transmit(&(hmc5883l->I2C), hmc5883l->compass_address,Trans,1);
    I2C_Receive(&(hmc5883l->I2C), hmc5883l->compass_address, Receive,1);

    if(LL_I2C_IsActiveFlag_RXNE(hmc5883l->I2C.I2C)|LL_I2C_IsActiveFlag_BUSY(hmc5883l->I2C.I2C))	// not empty, busy
    {
    	hmc5883l->status = 0;
    	hmc5883l->error_code = 1;

    	//while(HAL_I2C_GetState(&(hmc5883l->i2c)) != HAL_I2C_STATE_READY);

    }


    return Receive[0];
}



void Magneto_Init(HMC5883L* hmc5883l, I2C_TypeDef* i2c, double gauss)
{
	printf("Magneto_Init -> GET : 0x%X | Gauss : %f\r\n", hmc5883l->compass_address, gauss);

	hmc5883l->I2C.I2C = i2c;

	hmc5883l->error_code = SetScale(hmc5883l, gauss);
	if(hmc5883l->error_code == 1)
	{
		printf("Set Scale Error.\r\n");
	}
	hmc5883l->error_code = SetMeasurementMode(hmc5883l, Measurement_Continuous);
	if(hmc5883l->error_code == 1)
	{
		printf("Set Mode Error.\r\n");
	}

}



void ReadRawAxis(HMC5883L* hmc5883l, AxisData* compassData)
{
	uint8_t trans[1] = {DataRegisterBegin};
	uint8_t buffer[6];

	I2C_Transmit(&(hmc5883l->I2C), hmc5883l->compass_address, trans, 1);
	I2C_Receive(&(hmc5883l->I2C), hmc5883l->compass_address, buffer, sizeof(buffer));

	compassData->XAxis = (int16_t)(buffer[0] << 8) | buffer[1];
	compassData->YAxis = (int16_t)(buffer[2] << 8) | buffer[3];
	compassData->ZAxis = (int16_t)(buffer[4] << 8) | buffer[5];
}

void ReadScaledAxis(HMC5883L* hmc5883l, AxisData* compassData)
{
	compassData->scaled_XAxis = compassData->XAxis * hmc5883l->m_Scale;
	compassData->scaled_YAxis = compassData->YAxis * hmc5883l->m_Scale;
	compassData->scaled_ZAxis = compassData->ZAxis * hmc5883l->m_Scale;
}

void Read_Magneto(HMC5883L* hmc5883l, AxisData* compassData)
{
	uint8_t trans[1] = {DataRegisterBegin};
	uint8_t buffer[6];

	I2C_Transmit(&(hmc5883l->I2C), hmc5883l->compass_address, trans, 1);
	I2C_Receive(&(hmc5883l->I2C), hmc5883l->compass_address, buffer, sizeof(buffer));


	//HAL_I2C_Mem_Read(&(SENSOR->i2c), SENSOR->magneto_address , DataRegisterBegin, I2C_MEMADD_SIZE_8BIT, buffer, sizeof(buffer), 10);
	compassData->XAxis = (int16_t)(buffer[0] << 8) | buffer[1];
	compassData->YAxis = (int16_t)(buffer[2] << 8) | buffer[3];
	compassData->ZAxis = (int16_t)(buffer[4] << 8) | buffer[5];

	compassData->scaled_XAxis = compassData->XAxis * hmc5883l->m_Scale;
	compassData->scaled_YAxis = compassData->YAxis * hmc5883l->m_Scale;
	compassData->scaled_ZAxis = compassData->ZAxis * hmc5883l->m_Scale;

}



void Get_Heading_Magneto(AxisData* compassData)
{
	//static const double PI = 3.141592653589;
	float heading = atan2(compassData->scaled_YAxis, compassData->scaled_XAxis);
	//float declinationAngle = 0.0457;
	float declinationAngle = -0.1545;
    heading += declinationAngle;

    if(heading < 0) heading += 2*M_PI;

    if(heading > 2*M_PI) heading -= 2*M_PI;

    float headingDegrees = heading * 180/M_PI;

    compassData->heading = heading;
    compassData->headingDegrees = headingDegrees;
}


uint8_t SetScale(HMC5883L* hmc5883l, double gauss)
{
	uint8_t regValue = 0x00;

//	printf("SetScale gauss: %f | regValue : 0x%X\r\n", gauss, regValue);
//
//	if(gauss == 1.3)printf("float complete\r\n");
//	else printf("float not working\r\n");

	if(gauss == 0.88)
	{
		regValue = 0x00;
		hmc5883l->m_Scale = 0.73;
	}
	else if(gauss == 1.3)
	{
		regValue = 0x01;
		hmc5883l->m_Scale = 0.92;
		//printf("m_scale = %f \r\n", hmc5883l->m_Scale);
	}
	else if(gauss == 1.9)
	{
		regValue = 0x02;
		hmc5883l->m_Scale = 1.22;
	}
	else if(gauss == 2.5)
	{
		regValue = 0x03;
		hmc5883l->m_Scale = 1.52;
	}
	else if(gauss == 4.0)
	{
		regValue = 0x04;
		hmc5883l->m_Scale = 2.27;
	}
	else if(gauss == 4.7)
	{
		regValue = 0x05;
		hmc5883l->m_Scale = 2.56;
	}
	else if(gauss == 5.6)
	{
		regValue = 0x06;
		hmc5883l->m_Scale = 3.03;
	}
	else if(gauss == 8.1)
	{
		regValue = 0x07;
		hmc5883l->m_Scale = 4.35;
	}
	else{
		//printf("SetScale Func Error 0x%X\r\n", regValue);
		return ErrorCode_1_Num;
	}

	//printf("set Scale : %d\r\n", regValue);
	regValue = regValue << 5;
	Magneto_Writebyte(hmc5883l, ConfigurationRegisterB, regValue);

	return regValue;
}


uint8_t SetMeasurementMode(HMC5883L* hmc5883l , uint8_t mode)
{
	Magneto_Writebyte(hmc5883l, ModeRegister, mode);
	if((mode == 0x00) | (mode == 0x01) | (mode == 0x03)) return 0;
	else return ErrorCode_1_Num;

}





char* GetErrorText(uint8_t errorCode)
{
	if(ErrorCode_1_Num == 1)
		return ErrorCode_1;

	return "Error not defined.";
}
