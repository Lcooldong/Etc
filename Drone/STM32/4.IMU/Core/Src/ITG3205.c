/*
 * ITG3205.c
 *
 *  Created on: 2022. 5. 9.
 *      Author: user
 */

#include "ITG3205.h"



void Gyro_Writebyte(ITG3205* itg3205, uint8_t register_address, uint8_t data){
	/////////////////////////////////////////////////////////
	// |Start bit|AD+W|   |Register Address|   |DATA|   |P|//
	// |		 |	  |ACK|				   |ACK|	|ACK| |//
	/////////////////////////////////////////////////////////
	uint8_t Trans[2] = {register_address, data};

	I2C_Transmit(&(itg3205->I2C), itg3205->gyro_address, Trans, 2);

}

uint8_t Gyro_Readbyte(ITG3205* itg3205, uint8_t register_address){
	/////////////////////////////////////////////////////////////////////////////
	// |Start bit|AD+W|   |Register Address|   |Start Bit|AD+R|	  |	   |NACK|P|//
	// |		 |	  |ACK|				   |ACK|		 |	  |ACK|DATA|   	| |//
	/////////////////////////////////////////////////////////////////////////////
	uint8_t Trans[1] = {register_address};
	uint8_t Receive[1];

	I2C_Transmit(&(itg3205->I2C), itg3205->gyro_address, Trans, 1);
	I2C_Receive(&(itg3205->I2C), itg3205->gyro_address, Receive, 1);

	return Receive[0];
}



void Gyro_Init(ITG3205* itg3205, I2C_TypeDef* i2c){

	itg3205->I2C.I2C = i2c;
	itg3205->I2C.i2c_busy = 0;
	itg3205->I2C.i2c_IsRx = 0;

	printf("Gyro_Init -> GET    : 0x%X\r\n", itg3205->gyro_address);

    Gyro_Writebyte(itg3205,PWR_MGM,0x00);

    LL_mDelay(10);
    Gyro_Writebyte(itg3205, PWR_MGM,0x01);

    Gyro_Writebyte(itg3205, SMPLRT_DIV,0X00);

    Gyro_Writebyte(itg3205, DLPFFS_FS_SEL, RANGE2000 << 3);

    Gyro_Writebyte(itg3205, DLPFFS_DLPF_CFG, BW256_SR8);

    Gyro_Writebyte(itg3205, PWRMGM_CLK_SEL, PLL_ZGYRO_REF);

    Gyro_Writebyte(itg3205, INTCFG_ITG_RDY_EN, INTSTATUS_ITG_RDY);

    Gyro_Writebyte(itg3205, INTCFG_RAW_RDY_EN, INTSTATUS_RAW_DATA_RDY);

    LL_mDelay(GYROSTART_UP_DELAY);

}


void Read_Gyro(ITG3205* itg3205, GyroData* gyroData){
	uint8_t Trans[1] = {GYRO_XOUT};
    uint8_t gyro_buf[6];
    int16_t raw_X,raw_Y,raw_Z;

	I2C_Transmit(&(itg3205->I2C), itg3205->gyro_address, Trans, 1);
	I2C_Receive(&(itg3205->I2C), itg3205->gyro_address, gyro_buf, sizeof(gyro_buf));

    raw_X = ((gyro_buf[0]<<8)|gyro_buf[1]);
    raw_Y = ((gyro_buf[2]<<8)|gyro_buf[3]);
    raw_Z = ((gyro_buf[4]<<8)|gyro_buf[5]);

    gyroData->raw_gyro_X = raw_X;
    gyroData->raw_gyro_Y = raw_Y;
    gyroData->raw_gyro_Z = raw_Z;


    gyroData->gyro_X = ((float)(raw_X))/16.4 + gyroData->offset_X;
    gyroData->gyro_Y = ((float)(raw_Y))/16.4 + gyroData->offset_Y;
    gyroData->gyro_Z = ((float)(raw_Z))/16.4 + gyroData->offset_Z;
}

void Calibrate_Gyro(ITG3205* itg3205, GyroData* gyroData, uint16_t totSamples, uint16_t sampleDelayMS)
{
	printf("=Start Gyro Calibration=\r\n");
	int temp_X = 0, temp_Y = 0, temp_Z = 0;


	for(uint32_t i=0; i<totSamples; i++)
	{
		//printf("calibrate : %ld\r\n", i);
		LL_mDelay(sampleDelayMS);

		Read_Gyro(itg3205, gyroData);

		temp_X += gyroData->raw_gyro_X;
		temp_Y += gyroData->raw_gyro_Y;
		temp_Z += gyroData->raw_gyro_Z;
	}

	temp_X /= totSamples;
	temp_Y /= totSamples;
	temp_Z /= totSamples;

	gyroData->base_gyro_X = temp_X;
	gyroData->base_gyro_Y = temp_Y;
	gyroData->base_gyro_Z = temp_Z;

	gyroData->scaled_gyro_X = (gyroData->raw_gyro_X - gyroData->base_gyro_X)/DLPFFS_FS_SEL;	// FULL_SCALE
	gyroData->scaled_gyro_Y = (gyroData->raw_gyro_Y - gyroData->base_gyro_Y)/DLPFFS_FS_SEL;
	gyroData->scaled_gyro_Z = (gyroData->raw_gyro_Z - gyroData->base_gyro_Z)/DLPFFS_FS_SEL;


	printf("=End Gyro Calibration=\r\n");
}




void Read_Gyro_Temperature(ITG3205* itg3205, GyroData* gyroData)
{

	uint16_t raw_Temp;
	uint8_t temperature_offset = 31;

	uint8_t Trans[1] = {TEMP_OUT};
    uint8_t temp_buf[2];

	I2C_Transmit(&(itg3205->I2C), itg3205->gyro_address, Trans, 1);
	I2C_Receive(&(itg3205->I2C), itg3205->gyro_address, temp_buf, sizeof(temp_buf));

	raw_Temp = ((temp_buf[0]<<8)|temp_buf[1]);

	gyroData->gyro_Temp = ((raw_Temp -13200) / 280.0 -32 ) *5 /9 - temperature_offset;
	//itg3205->gyro_Temp = raw_Temp;
}



void Set_Gyro_ScaledFactor(ITG3205* itg3205, GyroData* gyroData, float _Xcoeff, float _Ycoeff, float _Zcoeff, bool _Radians)
{
	gyroData->scaleFactor_X = 14.375 * _Xcoeff;
	gyroData->scaleFactor_Y = 14.375 * _Ycoeff;
	gyroData->scaleFactor_Z = 14.375 * _Zcoeff;

	if(_Radians)
	{
		gyroData->scaleFactor_X /= 0.0174532925;
		gyroData->scaleFactor_Y /= 0.0174532925;
		gyroData->scaleFactor_Z /= 0.0174532925;
	}

}

void Set_Gyro_Offsets(GyroData* gyroData, uint16_t _Xoffset, uint16_t _Yoffset, uint16_t _Zoffset)
{
	gyroData->offset_X = _Xoffset;
	gyroData->offset_Y = _Yoffset;
	gyroData->offset_Z = _Zoffset;
}


uint8_t Get_Gyro_SampleRateDiv(ITG3205* itg3205)
{
	return Gyro_Readbyte(itg3205, SMPLRT_DIV);
}

void Set_Gyro_SampleRateDiv(ITG3205* itg3205, uint8_t _SampleRate)
{
	Gyro_Writebyte(itg3205, SMPLRT_DIV, _SampleRate);
}

uint8_t Get_Gyro_FSRange(ITG3205* itg3205)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, DLPF_FS);
	return (received_data[0] & DLPFFS_FS_SEL) >> 3;
}

void Set_Gyro_FSRange(ITG3205* itg3205, uint8_t _Range)
{
	uint8_t received_data[1];
	received_data[0] = Get_Gyro_FSRange(itg3205);
	Gyro_Writebyte(itg3205, DLPF_FS, (received_data[0] & ~DLPFFS_FS_SEL) |(_Range << 3) );
}

uint8_t Get_Gyro_FilterBW(ITG3205* itg3205)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, DLPF_FS);
	return (received_data[0] & DLPFFS_DLPF_CFG);
}

void Set_Gyro_FilterBW(ITG3205* itg3205, uint8_t _BW)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, DLPF_FS);
	Gyro_Writebyte(itg3205, DLPF_FS, (received_data[0] & ~DLPFFS_DLPF_CFG) | _BW);
}

bool is_Gyro_INTActiveOnLow(ITG3205* itg3205)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, INT_CFG);
	return ((received_data[0] & INTCFG_ACTL) >> 7);
}

void Set_Gyro_INTDriveType(ITG3205* itg3205, bool _State)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, INT_CFG);
	Gyro_Writebyte(itg3205, INT_CFG, ((received_data[0]& ~INTCFG_OPEN)| _State << 6) );
}

bool is_Gyro_LatchUntilCleard(ITG3205* itg3205)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, INT_CFG);
	return ((received_data[0] & INTCFG_LATCH_INT_EN)>> 5);
}

void Set_Gyro_LatchMode(ITG3205* itg3205, bool _State)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, INT_CFG);
	Gyro_Writebyte(itg3205, INT_CFG, ((received_data[0] & ~INTCFG_LATCH_INT_EN)| _State << 5));
}

bool is_Gyro_AnyRegClrMode(ITG3205* itg3205)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, INT_CFG);
	return ((received_data[0] & INTCFG_INT_ANYRD_2CLEAR) >> 4);
}

void Set_Gyro_LatchClearMode(ITG3205* itg3205, bool _State)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, INT_CFG);
	Gyro_Writebyte(itg3205, INT_CFG, ((received_data[0] & ~INTCFG_INT_ANYRD_2CLEAR) | _State << 4));
}

bool is_Gyro_RawDataReadyOn(ITG3205* itg3205)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, INT_CFG);
	return (received_data[0] & INTCFG_RAW_RDY_EN);
}

void Set_Gyro_RawDataReady(ITG3205* itg3205, bool _State)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, INT_CFG);
	Gyro_Writebyte(itg3205, INT_CFG, ((received_data[0] & ~INTCFG_RAW_RDY_EN)| _State));
}

bool is_Gyro_ITGReady(ITG3205* itg3205)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, INT_STATUS);
	return ((received_data[0] & INTSTATUS_ITG_RDY) >> 2);
}

bool is_Gyro_RawDataReady(ITG3205* itg3205)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, INT_STATUS);
	return (received_data[0] & INTSTATUS_RAW_DATA_RDY);
}




void Reset_Gyro(ITG3205* itg3205)
{
	Gyro_Writebyte(itg3205, PWR_MGM, PWRMGM_HRESET);
	HAL_Delay(GYROSTART_UP_DELAY);
}

bool is_Gyro_LowPower(ITG3205* itg3205)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, PWR_MGM);
	return ((received_data[0] & PWRMGM_SLEEP) >> 6) ;
}

void Set_Gyro_PowerMode(ITG3205* itg3205, bool _State)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, PWR_MGM);
	Gyro_Writebyte(itg3205, PWR_MGM, ((received_data[0]& ~PWRMGM_SLEEP) | _State << 6));
}

bool is_XgyroStandby(ITG3205* itg3205)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, PWR_MGM);
	return ((received_data[0] & PWRMGM_STBY_XG) >> 5);
}


bool is_YgyroStandby(ITG3205* itg3205)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, PWR_MGM);
	return ((received_data[0] & PWRMGM_STBY_YG) >> 4);
}

bool is_ZgyroStandby(ITG3205* itg3205)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, PWR_MGM);
	return ((received_data[0] & PWRMGM_STBY_ZG) >> 3);
}


void Set_XgyroStandby(ITG3205* itg3205, bool _Status)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, PWR_MGM);
	Gyro_Writebyte(itg3205, PWR_MGM, ((received_data[0] & PWRMGM_STBY_XG)| _Status << 5));
}

void Set_YgyroStandby(ITG3205* itg3205, bool _Status)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, PWR_MGM);
	Gyro_Writebyte(itg3205, PWR_MGM, ((received_data[0] & PWRMGM_STBY_YG)| _Status << 4));
}

void Set_ZgyroStandby(ITG3205* itg3205, bool _Status)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, PWR_MGM);
	Gyro_Writebyte(itg3205, PWR_MGM, ((received_data[0] & PWRMGM_STBY_ZG)| _Status << 3));
}


uint16_t Get_ClockSource(ITG3205* itg3205)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, PWR_MGM);
	return (received_data[0] & PWRMGM_CLK_SEL);
}

void Set_Gyro_ClockSource(ITG3205* itg3205, uint8_t _CLKsource)
{
	uint8_t received_data[1];
	received_data[0] = Gyro_Readbyte(itg3205, PWR_MGM);
	Gyro_Writebyte(itg3205, PWR_MGM, ((received_data[0] & ~PWRMGM_CLK_SEL)| _CLKsource));
}




































