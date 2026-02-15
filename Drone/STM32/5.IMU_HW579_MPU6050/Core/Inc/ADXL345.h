/*
 * ADXL345.h
 *
 *  Created on: 2022. 5. 9.
 *      Author: user
 */

#ifndef __MAIN_H
#include "main.h"
#endif

#ifndef __I2C_H__
#include "I2C.h"
#endif

#ifndef _MATH_H_
#include "math.h"
#endif

#ifndef _STDIO_H_
#include "stdio.h"
#endif

#ifndef _STDBOOL_H
#include "stdbool.h"
#endif

#ifndef INC_ADXL345_H_
#define INC_ADXL345_H_


/* ------- Register names ------- */
#define ADXL345_DEVID 0x00
#define ADXL345_RESERVED1 0x01
#define ADXL345_THRESH_TAP 0x1d
#define ADXL345_OFSX 0x1e
#define ADXL345_OFSY 0x1f
#define ADXL345_OFSZ 0x20
#define ADXL345_DUR 0x21
#define ADXL345_LATENT 0x22
#define ADXL345_WINDOW 0x23
#define ADXL345_THRESH_ACT 0x24
#define ADXL345_THRESH_INACT 0x25
#define ADXL345_TIME_INACT 0x26
#define ADXL345_ACT_INACT_CTL 0x27
#define ADXL345_THRESH_FF 0x28
#define ADXL345_TIME_FF 0x29
#define ADXL345_TAP_AXES 0x2a
#define ADXL345_ACT_TAP_STATUS 0x2b
#define ADXL345_BW_RATE 0x2c
#define ADXL345_POWER_CTL 0x2d
#define ADXL345_INT_ENABLE 0x2e
#define ADXL345_INT_MAP 0x2f
#define ADXL345_INT_SOURCE 0x30
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_DATAX0 0x32
#define ADXL345_DATAX1 0x33
#define ADXL345_DATAY0 0x34
#define ADXL345_DATAY1 0x35
#define ADXL345_DATAZ0 0x36
#define ADXL345_DATAZ1 0x37
#define ADXL345_FIFO_CTL 0x38
#define ADXL345_FIFO_STATUS 0x39


/* Data Rata Bandwidth (bw_code) */
#define ADXL345_BW_1600 0xF // 1111
#define ADXL345_BW_800  0xE // 1110
#define ADXL345_BW_400  0xD // 1101
#define ADXL345_BW_200  0xC // 1100
#define ADXL345_BW_100  0xB // 1011
#define ADXL345_BW_50   0xA // 1010
#define ADXL345_BW_25   0x9 // 1001
#define ADXL345_BW_12   0x8 // 1000
#define ADXL345_BW_6    0x7 // 0111
#define ADXL345_BW_3    0x6 // 0110



#define DATA_FORMAT_SELF_TEST 	0x80
#define DATA_FORMAT_SPI		  	0x40
#define DATA_FORMAT_INT_INVERT	0x20
#define DATA_FORMAT_FULL_RES	0x08
#define DATA_FORMAT_Justify		0x04
#define ADXL345_DATA_FORMAT_16g	0x03
#define ADXL345_DATA_FORMAT_8g	0x02
#define ADXL345_DATA_FORMAT_4g	0x01
#define ADXL345_DATA_FORMAT_2g 	0x00




/*
 Interrupt PINs
 INT1: 0
 INT2: 1
 */
#define ADXL345_INT1_PIN 0x00
#define ADXL345_INT2_PIN 0x01

/*
 Interrupt bit position
 */
#define ADXL345_INT_DATA_READY_BIT 0x07
#define ADXL345_INT_SINGLE_TAP_BIT 0x06
#define ADXL345_INT_DOUBLE_TAP_BIT 0x05
#define ADXL345_INT_ACTIVITY_BIT   0x04
#define ADXL345_INT_INACTIVITY_BIT 0x03
#define ADXL345_INT_FREE_FALL_BIT  0x02
#define ADXL345_INT_WATERMARK_BIT  0x01
#define ADXL345_INT_OVERRUNY_BIT   0x00

#define ADXL345_DATA_READY 0x07
#define ADXL345_SINGLE_TAP 0x06
#define ADXL345_DOUBLE_TAP 0x05
#define ADXL345_ACTIVITY   0x04
#define ADXL345_INACTIVITY 0x03
#define ADXL345_FREE_FALL  0x02
#define ADXL345_WATERMARK  0x01
#define ADXL345_OVERRUNY   0x00




#define ADXL345_OK    1 // no error
#define ADXL345_ERROR 0 // indicates error is predent

#define ADXL345_NO_ERROR   0 // initial state
#define ADXL345_READ_ERROR 1 // problem reading accel
#define ADXL345_BAD_ARG    2 // bad method argument


typedef struct __ADXL345{
	I2C_struct I2C;
	uint8_t accel_address;

	uint8_t status;
	uint8_t error_code;

}ADXL345;


typedef struct __AccelData{

	int8_t offset_X;
	int8_t offset_Y;
	int8_t offset_Z;

	int16_t raw_accel_X;
	int16_t raw_accel_Y;
	int16_t raw_accel_Z;

	double gain_X;
	double gain_Y;
	double gain_Z;

	double scaled_accel_X;
	double scaled_accel_Y;
	double scaled_accel_Z;

}AccelData;


void Accel_Writebyte(ADXL345 * adxl345, uint8_t register_address, uint8_t data);
uint8_t Accel_Readbyte(ADXL345 * adxl345,uint8_t register_address);
void Accel_Init(ADXL345* adxl345, I2C_TypeDef* i2c);
void Read_Accel(ADXL345* adxl345, AccelData* accelData);
void Get_Accel(ADXL345* adxl345);

uint8_t Get_ACCEL_RangeSetting(ADXL345* adxl345, uint8_t rangeSetting);
void Set_ACCEL_RangeSetting(ADXL345* adxl345, uint8_t val);

bool Get_ACCEL_SelfTestBit(ADXL345* adxl345);
void Set_ACCEL_SelfTestBit(ADXL345* adxl345, bool selfTestBit);

bool Get_ACCEL_SpitBit(ADXL345* adxl345);
void Set_ACCEL_SpiBit(ADXL345* adxl345, bool spiBit);

bool Get_ACCEL_InterruptLeveltBit(ADXL345* adxl345);
void Set_ACCEL_InterruptLeveltBit(ADXL345* adxl345, bool interruptLevelBit);

bool Get_ACCEL_FullResBit(ADXL345* adxl345);
void Set_ACCEL_FullResBit(ADXL345* adxl345, bool fullResBit);

bool Get_ACCEL_JustifyBit(ADXL345* adxl345);
void Set_ACCEL_JustifyBit(ADXL345* adxl345, bool justifyBit);

uint8_t Get_ACCEL_TapThreadhold(ADXL345* adxl345);
void Set_ACCEL_TapThreadhold(ADXL345* adxl345 ,uint8_t tapThreadhold);

double* Get_ACCEL_AxisGain(AccelData* accelData, double array[3]);
void Set_ACCEL_AxisGain(AccelData* accelData, double array[3]);

int8_t* Get_ACCEL_AxisOffset(ADXL345* adxl345, AccelData* accelData, int8_t received_data[3]);
void Set_ACCEL_AxisOffset(ADXL345* adxl345, AccelData* accelData, int8_t _Xoffset, int8_t _Yoffset, int8_t _Zoffset);

uint8_t Get_ACCEL_TapDuration(ADXL345* adxl345);
void Set_ACCEL_TapDuration(ADXL345* adxl345, uint8_t tapDuration);

uint8_t Get_ACCEL_DoubleTapLatency(ADXL345* adxl345);
void Set_ACCEL_DoubleTapLatency(ADXL345* adxl345, uint8_t doubleTapLatency);

uint8_t Get_ACCEL_DoubleTapWindow(ADXL345* adxl345);
void Set_ACCEL_DoubleTapWindow(ADXL345* adxl345, uint8_t doubleTapWindow);

uint8_t Get_ACCEL_ActivityThreshold(ADXL345* adxl345);
void Set_ACCEL_ActivityThreshold(ADXL345* adxl345, uint8_t activityThreshold);

uint8_t Get_ACCEL_InactivityThreshold(ADXL345* adxl345);
void Set_ACCEL_InactivityThreshold(ADXL345* adxl345, uint8_t inactivityThreshold);

uint8_t Get_ACCEL_TimeInactivity(ADXL345* adxl345);
void Set_ACCEL_TimeInactivity(ADXL345* adxl345, uint8_t timeInactivity);

uint8_t Get_ACCEL_FreeFallThreshold(ADXL345* adxl345);
void Set_ACCEL_FreeFallThreshold(ADXL345* adxl345, uint8_t freeFallThreshold);

uint8_t Get_ACCEL_FreeFallDuration(ADXL345* adxl345);
void Set_ACCEL_FreeFallDuration(ADXL345* adxl345, uint8_t freeFallDuration);

bool is_ACCEL_ActivityXEnabled(ADXL345* adxl345);
bool is_ACCEL_ActivityYEnabled(ADXL345* adxl345);
bool is_ACCEL_ActivityZEnabled(ADXL345* adxl345);

bool is_ACCEL_InactivityXEnabled(ADXL345* adxl345);
bool is_ACCEL_InactivityYEnabled(ADXL345* adxl345);
bool is_ACCEL_InactivityZEnabled(ADXL345* adxl345);

void Set_ACCEL_ActivityX(ADXL345* adxl345, bool _State);
void Set_ACCEL_ActivityY(ADXL345* adxl345, bool _State);
void Set_ACCEL_ActivityZ(ADXL345* adxl345, bool _State);

void Set_ACCEL_InactivityX(ADXL345* adxl345, bool _State);
void Set_ACCEL_InactivityY(ADXL345* adxl345, bool _State);
void Set_ACCEL_InactivityZ(ADXL345* adxl345, bool _State);

bool is_ACCEL_ActivityAc(ADXL345* adxl345);
bool is_ACCEL_InactivityAc(ADXL345* adxl345);
void Set_ACCEL_ActivityAc(ADXL345* adxl345, bool _State);
void Set_ACCEL_InactivityAc(ADXL345* adxl345, bool _State);

bool Get_ACCEL_SuppressBit(ADXL345* adxl345);
void Set_ACCEL_SuppressBit(ADXL345* adxl345, bool _State);

bool is_ACCEL_TapDetectionOnX(ADXL345* adxl345);
void Set_ACCEL_TapDetectionOnX(ADXL345* adxl345, bool _State);
bool is_ACCEL_TapDetectionOnY(ADXL345* adxl345);
void Set_ACCEL_TapDetectionOnY(ADXL345* adxl345, bool _State);
bool is_ACCEL_TapDetectionOnZ(ADXL345* adxl345);
void Set_ACCEL_TapDetectionOnZ(ADXL345* adxl345, bool _State);

bool is_ACCEL_ActivitySourceOnX(ADXL345* adxl345);
bool is_ACCEL_ActivitySourceOnY(ADXL345* adxl345);
bool is_ACCEL_ActivitySourceOnZ(ADXL345* adxl345);

bool is_ACCEL_TapSourceOnX(ADXL345* adxl345);
bool is_ACCEL_TapSourceOnY(ADXL345* adxl345);
bool is_ACCEL_TapSourceOnZ(ADXL345* adxl345);

bool is_ACCEL_Asleep(ADXL345* adxl345);
bool is_ACCEL_LowPower(ADXL345* adxl345);
void Set_ACCEL_LowPower(ADXL345* adxl345, bool _State);

double Get_ACCEL_Rate(ADXL345* adxl345);
void Set_ACCEL_Rate(ADXL345* adxl345, double rate);

uint8_t Get_ACCEL_bw_code(ADXL345* adxl345);
void Set_ACCEL_bw_code(ADXL345* adxl345, uint8_t bw_code);

bool ACCEL_triggered(uint8_t interrupts, uint8_t mask);
uint8_t Get_ACCEL_InterruptSource(ADXL345* adxl345);
bool Get_ACCEL_InterruptMapping(ADXL345* adxl345, uint8_t interruptBit);
void Set_ACCEL_InterruptMapping(ADXL345* adxl345, uint8_t interruptBit, bool interruptPin);
bool is_ACCEL_InterruptEnabled(ADXL345* adxl345, uint8_t interruptBit);
void Set_ACCEL_Interrupt(ADXL345* adxl345, uint8_t interruptBit, bool _State);

bool Get_ACCEL_RegisterBit(ADXL345* adxl345, uint8_t register_address, uint8_t bitPos);
void Set_ACCEL_RegisterBit(ADXL345* adxl345, uint8_t register_address, uint8_t bitPos, bool _State);

void printAllRegister(ADXL345* adxl345);
void print_byte(uint8_t val);

#endif /* INC_ADXL345_H_ */














