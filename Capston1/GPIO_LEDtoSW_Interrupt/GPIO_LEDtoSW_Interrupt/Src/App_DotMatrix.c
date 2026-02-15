/*
 * App_DotMatrix.c
 *
 * Created: 2019-11-12 오후 2:39:00
 *  Author: Hybus_EDU_3
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
#include "Drv_GPIO.h"
#include "App_DotMatrix.h"
/* USER CODE END Includes */

/******************************************************
	   COL0    COL1    COL2    COL3    COL4	
ROW0	1		2		3		4		5
	
ROW1	6		7		8		9		10
	
ROW2	11		12		13		14		15
	
ROW3	16		17		18		19		20
	
ROW4	21		22		23		24		25
	
ROW5	26		27		28		29		30
	
ROW6	31		32		33		34		35

******************************************************/
uint8_t DotTestArr[DOT_MAX_VALUE] = {3,7,8,13,18,23,28,32,33,34};	//숫자 1만들기 위한 배열
	
uint8_t DotTestRowArr[ROW][COLUMN] = {
	{1,2,3,4,5}, 
	{6,7,8,9,10}, 
	{11,12,13,14,15}, 
	{16,17,18,19,20}, 
	{21,22,23,24,25}, 
	{26,27,28,29,30}, 
	{31,32,33,34,35}
};


uint8_t DotTestRowValue = DOT_CLEAR_VALUE;
uint8_t DotNumState = 0;
uint8_t DotNumValue = 9;

void DotMatrix_IO_Init()
{
	//DDRD |= 0b 0111 0000
	DOT_DAT_DDRD |= (1<<DOT_DAT0_PORTD4) | (1<<DOT_DAT1_PORTD5) | (1<<DOT_DAT2_PORTD6);
	
	//PORTD &= ~(0b 0111 0000)	=>	PORTD = 0b x000 xxxx
	DOT_DAT_PORTD &= ~( (1<<DOT_DAT0_PORTD4) | (1<<DOT_DAT1_PORTD5) | (1<<DOT_DAT2_PORTD6) );
	
	
	//DDRE |= 0b 1100 0000
	DOT_CS_DDRE |= (1<<DOT_CS0_PORTE6) | (1<<DOT_CS1_PORTE7);
	
	//PORTE &= ~(0b 1100 0000)	=>	PORTE = 0b 00xx xxxx
	DOT_CS_PORTE &= ~( (1<<DOT_CS0_PORTE6) | (1<<DOT_CS1_PORTE7) );
}


void WrDotMatrixCol(uint8_t Data)
{
	DOT_DAT_PORTD = (Data << 4);
	
	delay_us(2);
	COL_CLK_ON;
	delay_us(2);
	COL_CLK_OFF
	delay_us(2);
}

void WrDotMatrixRow(uint8_t Data)
{
	DOT_DAT_PORTD = (Data << 4);
	
	delay_us(2);
	ROW_CLK_ON;
	delay_us(2);
	ROW_CLK_OFF
	delay_us(2);	
}

void WrDotMatrixClear()
{
	//Row 8번째 비트는 사용하지 않기에 해당 비트를 선택하면 나머지 비트가 High가 된다.
	//그로 인해 Col 비트와 관계없이 어떠한 도트도 동작하지 않게 된다.
	WrDotMatrixRow(7);
	
	//Col 또한 마찬가지
	WrDotMatrixCol(7);
}

uint8_t WrDotMatrixPoint(uint8_t Point)
{
	uint8_t Col = 0;
	uint8_t Row = 0;
	
	if(Point < DOT_MIN_VALUE || Point > DOT_MAX_VALUE){
		WrDotMatrixRow(7);
		return 1;
	}
	
	Col = (Point - 1) % 5;
	Row = (Point - 1) / 5;
	
	WrDotMatrixCol(Col);
	WrDotMatrixRow(Row);
	
	return 0;
}

void WrDotMatrixPointArry(uint8_t *PointArr, uint8_t ArrSize)
{
	uint8_t Index = 0;
	
	for(Index = 0; Index < ArrSize; Index++){
		WrDotMatrixPoint(*(PointArr+Index));
	}	
}



void WrDotMatrixRowArry(uint8_t rowValue)
{
	switch(rowValue){
		case ROW0 :
			WrDotMatrixPointArry(DotTestRowArr[ROW0], sizeof(DotTestRowArr[ROW0]));
		break;
		case ROW1 :
			WrDotMatrixPointArry(DotTestRowArr[ROW1], sizeof(DotTestRowArr[ROW1]));
		break;
		case ROW2 :
			WrDotMatrixPointArry(DotTestRowArr[ROW2], sizeof(DotTestRowArr[ROW2]));
		break;
		case ROW3 :
			WrDotMatrixPointArry(DotTestRowArr[ROW3], sizeof(DotTestRowArr[ROW3]));
		break;
		case ROW4 :
			WrDotMatrixPointArry(DotTestRowArr[ROW4], sizeof(DotTestRowArr[ROW4]));
		break;
		case ROW5 :
			WrDotMatrixPointArry(DotTestRowArr[ROW5], sizeof(DotTestRowArr[ROW5]));
		break;
		case ROW6 :
			WrDotMatrixPointArry(DotTestRowArr[ROW6], sizeof(DotTestRowArr[ROW6]));
		break;
		default:
			WrDotMatrixClear();
		break;
	}
}

void WrDotMatrixNumDisPlay(uint8_t Num)
{
	switch(Num)
	{
		case 0 :	WrDotMatrixNum(DotArrNum0);		break;
		case 1 :	WrDotMatrixNum(DotArrNum1);		break;
		case 2 :	WrDotMatrixNum(DotArrNum2);		break;
		case 3 :	WrDotMatrixNum(DotArrNum3);		break;
		case 4 :	WrDotMatrixNum(DotArrNum4);		break;
		case 5 :	WrDotMatrixNum(DotArrNum5);		break;
		case 6 :	WrDotMatrixNum(DotArrNum6);		break;
		case 7 :	WrDotMatrixNum(DotArrNum7);		break;
		case 8 :	WrDotMatrixNum(DotArrNum8);		break;
		case 9 :	WrDotMatrixNum(DotArrNum9);		break;
	}
}

void WrDotMatrixNum(uint8_t (*DotNumArr)[5])
{
	for(int i=0; i<7; i++)
	{
		WrDotMatrixClear();
		WrDotMatrixRow(i);
		for(int j=0; j<5; j++)
		{
			if(DotNumArr[i][j]){
				WrDotMatrixCol(j);
			}
		}
	}
}

uint8_t DotArrNum0[ROW][COLUMN] = {
	{0,1,1,1,0},
	{1,0,0,0,1},
	{1,0,0,0,1},
	{1,0,0,0,1},
	{1,0,0,0,1},
	{1,0,0,0,1},
	{0,1,1,1,0},
};

uint8_t DotArrNum1[ROW][COLUMN] = {
	{0,0,1,0,0},
	{0,1,1,0,0},
	{0,0,1,0,0},
	{0,0,1,0,0},
	{0,0,1,0,0},
	{0,0,1,0,0},
	{0,1,1,1,0},
};

uint8_t DotArrNum2[ROW][COLUMN] = {
	{1,1,1,1,1},
	{0,0,0,0,1},
	{0,0,0,0,1},
	{1,1,1,1,1},
	{1,0,0,0,0},
	{1,0,0,0,0},
	{1,1,1,1,1},
};

uint8_t DotArrNum3[ROW][COLUMN] = {
	{1,1,1,1,1},
	{0,0,0,0,1},
	{0,0,0,0,1},
	{1,1,1,1,1},
	{0,0,0,0,1},
	{0,0,0,0,1},
	{1,1,1,1,1},
};

uint8_t DotArrNum4[ROW][COLUMN] = {
	{1,0,0,0,1},
	{1,0,0,0,1},
	{1,0,0,0,1},
	{1,1,1,1,1},
	{0,0,0,0,1},
	{0,0,0,0,1},
	{0,0,0,0,1},
};

uint8_t DotArrNum5[ROW][COLUMN] = {
	{1,1,1,1,1},
	{1,0,0,0,0},
	{1,0,0,0,0},
	{1,1,1,1,1},
	{0,0,0,0,1},
	{0,0,0,0,1},
	{1,1,1,1,1},
};

uint8_t DotArrNum6[ROW][COLUMN] = {
	{1,1,1,1,1},
	{1,0,0,0,0},
	{1,0,0,0,0},
	{1,1,1,1,1},
	{1,0,0,0,1},
	{1,0,0,0,1},
	{1,1,1,1,1},
};

uint8_t DotArrNum7[ROW][COLUMN] = {
	{1,1,1,1,1},
	{1,0,0,0,1},
	{1,0,0,0,1},
	{0,0,0,0,1},
	{0,0,0,0,1},
	{0,0,0,0,1},
	{0,0,0,0,1},
};

uint8_t DotArrNum8[ROW][COLUMN] = {
	{1,1,1,1,1},
	{1,0,0,0,1},
	{1,0,0,0,1},
	{1,1,1,1,1},
	{1,0,0,0,1},
	{1,0,0,0,1},
	{1,1,1,1,1},
};

uint8_t DotArrNum9[ROW][COLUMN] = {
	{1,1,1,1,1},
	{1,0,0,0,1},
	{1,0,0,0,1},
	{1,1,1,1,1},
	{0,0,0,0,1},
	{0,0,0,0,1},
	{1,1,1,1,1},
};