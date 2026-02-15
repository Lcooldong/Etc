/*
 * App_DotMatrix.h
 *
 * Created: 2019-11-12 오후 2:39:18
 *  Author: Hybus_EDU_3
 */ 


#ifndef APP_DOTMATRIX_H_
#define APP_DOTMATRIX_H_


#define DOT_DAT_DDRD		DDRD
#define DOT_DAT_PORTD		PORTD
#define DOT_DAT0_PORTD4		PD4
#define DOT_DAT1_PORTD5		PD5
#define DOT_DAT2_PORTD6		PD6

#define DOT_CS_DDRE			DDRE
#define DOT_CS_PORTE		PORTE
#define DOT_CS0_PORTE6		PE6
#define DOT_CS1_PORTE7		PE7

#define COLUMN	5
#define COL0	0
#define COL1	1
#define COL2	2
#define COL3	3
#define COL4	4

#define ROW		7
#define ROW0	0
#define ROW1	1
#define ROW2	2
#define ROW3	3
#define ROW4	4
#define ROW5	5
#define ROW6	6

#define DOT_MIN_VALUE	1
#define DOT_MAX_VALUE	35

#define DOT_CLEAR_VALUE	0xFF

#define COL_CLK_ON		GPIO_WritePin(&DOT_CS_PORTE, DOT_CS0_PORTE6, GPIO_PIN_SET);
#define COL_CLK_OFF		GPIO_WritePin(&DOT_CS_PORTE, DOT_CS0_PORTE6, GPIO_PIN_RESET);

#define ROW_CLK_ON		GPIO_WritePin(&DOT_CS_PORTE, DOT_CS1_PORTE7, GPIO_PIN_SET);
#define ROW_CLK_OFF		GPIO_WritePin(&DOT_CS_PORTE, DOT_CS1_PORTE7, GPIO_PIN_RESET);


extern uint8_t DotTestArr[DOT_MAX_VALUE];
extern uint8_t DotTestRowArr[ROW][COLUMN];
extern uint8_t DotTestRowValue;
extern uint8_t DotArrNum0[ROW][COLUMN];
extern uint8_t DotArrNum1[ROW][COLUMN];
extern uint8_t DotArrNum2[ROW][COLUMN];
extern uint8_t DotArrNum3[ROW][COLUMN];
extern uint8_t DotArrNum4[ROW][COLUMN];
extern uint8_t DotArrNum5[ROW][COLUMN];
extern uint8_t DotArrNum6[ROW][COLUMN];
extern uint8_t DotArrNum7[ROW][COLUMN];
extern uint8_t DotArrNum8[ROW][COLUMN];
extern uint8_t DotArrNum9[ROW][COLUMN];

extern uint8_t DotNumState;
extern uint8_t DotNumValue;

void DotMatrix_IO_Init();
void WrDotMatrixCol(uint8_t Data);
void WrDotMatrixRow(uint8_t Data);
void WrDotMatrixClear();
uint8_t WrDotMatrixPoint(uint8_t Point);
void WrDotMatrixPointArry(uint8_t *PointArr, uint8_t ArrSize);
void WrDotMatrixRowArry(uint8_t rowValue);

void WrDotMatrixNumDisPlay(uint8_t Num);
void WrDotMatrixNum(uint8_t (*DotNumArr)[5]);

#endif /* APP_DOTMATRIX_H_ */