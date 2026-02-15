/*
 * SPLC780D1 Driver
 *
 * Ported By	| Evan Harvey <pocopoco@hybus.net>
 * Developed By | Tilen Majerle <tilen.majerle@gmail.com>
 *
 * Copyright (c) 2019 HyBus
 * Copyright (c) 2016 Tilen Majerle
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * -------------------------------------------------------------
 * Dependent Libraries
 * -------------------------------------------------------------
 *
 * DWT for microsecond delay
 * https://github.com/danielbalster/a500kbd-usbhid-stm32/blob/master/Src/dwt_stm32_delay.c
 * https://github.com/danielbalster/a500kbd-usbhid-stm32/blob/master/Src/dwt_stm32_delay.h
 */

#include "drivers/drv_splc780d1.h"
#include "drivers/dwt_stm32_delay.h"

#define SPLC780D1_DELAY(x)	DWT_Delay_us(x)
#define SPLC780D1_RS_LOW 	HAL_GPIO_WritePin(SPLC780D1_RS_PORT, SPLC780D1_RS_PIN, GPIO_PIN_RESET)
#define SPLC780D1_RS_HIGH 	HAL_GPIO_WritePin(SPLC780D1_RS_PORT, SPLC780D1_RS_PIN, GPIO_PIN_SET)
#define SPLC780D1_EN_LOW 	HAL_GPIO_WritePin(SPLC780D1_EN_PORT, SPLC780D1_EN_PIN, GPIO_PIN_RESET)
#define SPLC780D1_EN_HIGH 	HAL_GPIO_WritePin(SPLC780D1_EN_PORT, SPLC780D1_EN_PIN, GPIO_PIN_SET)
#define SPLC780D1_EN_BLINK	SPLC780D1_EN_HIGH; SPLC780D1_DELAY(20); SPLC780D1_EN_LOW; SPLC780D1_DELAY(20);

static SPLC780D1_Options_t SPLC780D1_Opts;

/**
 * @brief	Initializes data and status pins.
 */
void SPLC780D1_PinInit(void) {
	HAL_GPIO_WritePin(SPLC780D1_D4_PORT, SPLC780D1_D4_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SPLC780D1_D5_PORT, SPLC780D1_D5_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SPLC780D1_D6_PORT, SPLC780D1_D6_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SPLC780D1_D7_PORT, SPLC780D1_D7_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SPLC780D1_RS_PORT, SPLC780D1_RS_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SPLC780D1_EN_PORT, SPLC780D1_EN_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SPLC780D1_RW_PORT, SPLC780D1_RW_PIN, GPIO_PIN_RESET);
}

/**
 * @brief	Initialize SPLC780D1 Controller.
 */
void SPLC780D1_Init(uint8_t cols, uint8_t rows) {
	/* Init pinout */
	SPLC780D1_PinInit();

	/* At least 40ms ( +5ms offset ) */
	SPLC780D1_DELAY(45000);

	/* Set LCD width and height */
	SPLC780D1_Opts.Rows = rows;
	SPLC780D1_Opts.Cols = cols;
	SPLC780D1_Opts.currentX = 0;
	SPLC780D1_Opts.currentY = 0;

	/* Set display function */
	SPLC780D1_Opts.DisplayFunction = SPLC780D1_4BITMODE | SPLC780D1_5x8DOTS | SPLC780D1_1LINE;
	if (rows > 1)
		SPLC780D1_Opts.DisplayFunction |= SPLC780D1_2LINE;

	/* Try to set 4-bit mode */
	SPLC780D1_Write4Bit(0x03);
	SPLC780D1_DELAY(4000);

	/* Second try */
	SPLC780D1_Write4Bit(0x03);
	SPLC780D1_DELAY(4000);

	/* Third try */
	SPLC780D1_Write4Bit(0x03);
	SPLC780D1_DELAY(4000);

	/* Set 4-bit interface */
	SPLC780D1_Write4Bit(0x02);
	SPLC780D1_DELAY(100);

	/* Set lines, font size, etc. */
	SPLC780D1_Cmd(SPLC780D1_FUNCTIONSET | SPLC780D1_Opts.DisplayFunction);

	/* Turn the display on with no cursor or blinking default */
	SPLC780D1_Opts.DisplayControl = SPLC780D1_DISPLAYON;
	SPLC780D1_DisplayOn();

	/* Clear LCD */
	SPLC780D1_Clear();

	/* Default font directions */
	SPLC780D1_Opts.DisplayMode = SPLC780D1_ENTRYLEFT | SPLC780D1_ENTRYSHIFTDECREMENT;
	SPLC780D1_Cmd(SPLC780D1_ENTRYMODESET | SPLC780D1_Opts.DisplayMode);

	/* Delay */
	SPLC780D1_DELAY(4000);

}

/*
 * @brief	Put the text at x, y cursor.
 */
void SPLC780D1_Put(uint8_t x, uint8_t y, char* str) {
	SPLC780D1_CursorSet(x, y);

	while (*str) {
		if (SPLC780D1_Opts.currentX >= SPLC780D1_Opts.Cols) {
			SPLC780D1_Opts.currentX = 0;
			SPLC780D1_Opts.currentY++;
			SPLC780D1_CursorSet(SPLC780D1_Opts.currentX, SPLC780D1_Opts.currentY);
		}

		if (*str == '\n') {
			SPLC780D1_Opts.currentY++;
			SPLC780D1_CursorSet(SPLC780D1_Opts.currentX, SPLC780D1_Opts.currentY);
		} else if (*str == '\r') {
			SPLC780D1_CursorSet(0, SPLC780D1_Opts.currentY);
		} else {
			SPLC780D1_Data(*str);
			SPLC780D1_Opts.currentX++;
		}

		str++;
	}
}

/*
 * @brief Clear display.
 */
void SPLC780D1_Clear(void) {
	SPLC780D1_Cmd(SPLC780D1_CLEARDISPLAY);
	SPLC780D1_DELAY(3000);
}

/*
 * @brief Display on.
 */
void SPLC780D1_DisplayOn(void) {
	SPLC780D1_Opts.DisplayControl |= SPLC780D1_DISPLAYON;
	SPLC780D1_Cmd(SPLC780D1_DISPLAYCONTROL | SPLC780D1_Opts.DisplayControl);
}


/*
 * @brief Display off.
 */
void SPLC780D1_DisplayOff(void) {
	SPLC780D1_Opts.DisplayControl &= ~SPLC780D1_DISPLAYON;
	SPLC780D1_Cmd(SPLC780D1_DISPLAYCONTROL | SPLC780D1_Opts.DisplayControl);
}

/*
 * @brief Blink on.
 */
void SPLC780D1_BlinkOn(void) {
	SPLC780D1_Opts.DisplayControl |= SPLC780D1_BLINKON;
	SPLC780D1_Cmd(SPLC780D1_DISPLAYCONTROL | SPLC780D1_Opts.DisplayControl);
}

/*
 * @brief Blink off.
 */
void SPLC780D1_BlinkOff(void) {
	SPLC780D1_Opts.DisplayControl &= ~SPLC780D1_BLINKON;
	SPLC780D1_Cmd(SPLC780D1_DISPLAYCONTROL | SPLC780D1_Opts.DisplayControl);
}

/*
 * @brief Cursor on.
 */
void SPLC780D1_CursorOn(void) {
	SPLC780D1_Opts.DisplayControl |= SPLC780D1_CURSORON;
	SPLC780D1_Cmd(SPLC780D1_DISPLAYCONTROL | SPLC780D1_Opts.DisplayControl);
}

/*
 * @brief Cursor off.
 */
void SPLC780D1_CursorOff(void) {
	SPLC780D1_Opts.DisplayControl |= ~SPLC780D1_CURSORON;
	SPLC780D1_Cmd(SPLC780D1_DISPLAYCONTROL | SPLC780D1_Opts.DisplayControl);
}

/*
 * @brief Scroll left.
 */
void SPLC780D1_ScrollLeft(void) {
	SPLC780D1_Cmd(SPLC780D1_CURSORSHIFT | SPLC780D1_DISPLAYMOVE | SPLC780D1_MOVELEFT);
}

/*
 * @brief Scroll right.
 */
void SPLC780D1_ScrollRight(void) {
	SPLC780D1_Cmd(SPLC780D1_CURSORSHIFT | SPLC780D1_DISPLAYMOVE | SPLC780D1_MOVERIGHT);
}

/*
 * @brief Set cursor.
 */
void SPLC780D1_CursorSet(uint8_t col, uint8_t row) {
	uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};

	if (row >= SPLC780D1_Opts.Rows)
		row = 0;

	SPLC780D1_Opts.currentX = col;
	SPLC780D1_Opts.currentY = row;

	SPLC780D1_Cmd(SPLC780D1_SETDDRAMADDR | (col + row_offsets[row]));
}

/*
 * Data transmit function
 *
 * - Data structure
 * D7 | <D7> <D3>
 * D6 | <D6> <D2>
 * D5 | <D5> <D1>
 * D4 | <D4> <D0>
 *
 * - Sequence
 * 1. Call SPLC780D1_Cmd or SPLC780D1_Data.
 * 2. If command trasnmit, RS Pin set high. If data transmit, RS pin set low.
 * 3. And call SPLC780D1_Write function.
 * 4. This function call SPLC780D1_Write4Bit. ( See the data structure. )
 */

/*
 * @brief Send command to controller.
 */
void SPLC780D1_Cmd(uint8_t value) {
	SPLC780D1_RS_LOW;
	SPLC780D1_Write(value);
}

/*
 * @brief Send data to controller.
 */
void SPLC780D1_Data(uint8_t value) {
	SPLC780D1_RS_HIGH;
	SPLC780D1_Write(value);
}

/*
 * @brief Write byte to controller.
 */
void SPLC780D1_Write(uint8_t value) {
	SPLC780D1_Write4Bit(value >> 4);
	SPLC780D1_Write4Bit(value & 0x0F);
}

/*
 * @brief Write 4bit to controller.
 */
void SPLC780D1_Write4Bit(uint8_t value) {
	HAL_GPIO_WritePin(SPLC780D1_D7_PORT, SPLC780D1_D7_PIN, (value & 0x08));
	HAL_GPIO_WritePin(SPLC780D1_D6_PORT, SPLC780D1_D6_PIN, (value & 0x04));
	HAL_GPIO_WritePin(SPLC780D1_D5_PORT, SPLC780D1_D5_PIN, (value & 0x02));
	HAL_GPIO_WritePin(SPLC780D1_D4_PORT, SPLC780D1_D4_PIN, (value & 0x01));
	SPLC780D1_EN_BLINK;
}

/*
 * @brief Create custom character.
 */
void SPLC780D1_CreateChar(uint8_t loc, uint8_t *data) {
	uint8_t i;

	loc &= 0x07;
	SPLC780D1_Cmd(SPLC780D1_SETCGRAMADDR | (loc << 3));

	for (i = 0; i < 8; i++) {
		SPLC780D1_Data(data[i]);
	}
}

/*
 * @brief Put custom character to display.
 */
void SPLC780D1_PutCustom(uint8_t x, uint8_t y, uint8_t loc) {
	SPLC780D1_CursorSet(x, y);
	SPLC780D1_Data(loc);
}
