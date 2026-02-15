/*
 * PCA9535 Driver
 *
 * Developed By | Evan Harvey <pocopoco@hybus.net>
 *
 * Copyright (c) 2019 HyBus
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

#ifndef PCA9535_H
#define PCA9535_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/*
 * Define
 */

typedef struct {
	I2C_HandleTypeDef * handle;
	uint8_t				addr;
	uint8_t				portData;
	uint8_t				outData;
	uint8_t				tmpData[1];
} PCA9535_HandleTypeDef;

#define PCA9535_INPUT_PORT0		0x00
#define PCA9535_INPUT_PORT1		0x01
#define PCA9535_OUTPUT_PORT0	0x02
#define PCA9535_OUTPUT_PORT1	0x03
#define PCA9535_POLARITY_PORT0	0x04
#define PCA9535_POLARITY_PORT1	0x05
#define PCA9535_CONFIG_PORT0	0x06
#define PCA9535_CONFIG_PORT1	0x07

#define PCA9535_PORT_0			0x01
#define PCA9535_PORT_1			0x02

#define PCA9535_PIN_0			0x01
#define PCA9535_PIN_1			0x02
#define PCA9535_PIN_2			0x04
#define PCA9535_PIN_3			0x08
#define PCA9535_PIN_4			0x10
#define PCA9535_PIN_5			0x20
#define PCA9535_PIN_6			0x40
#define PCA9535_PIN_7			0x80

#define PCA9535_OUTPUT			0x00
#define PCA9535_INPUT			0x01

/*
 * Prototype function
 */

void PCA9535_Init
(
	PCA9535_HandleTypeDef * handle,
	I2C_HandleTypeDef * i2c_handle,
	uint8_t addr
);

void PCA9535_WriteData
(
	PCA9535_HandleTypeDef * handle,
	uint8_t cmd,
	uint8_t data
);

uint8_t PCA9535_ReadData
(
	PCA9535_HandleTypeDef * handle,
	uint8_t cmd
);

void PCA9535_PinMode
(
	PCA9535_HandleTypeDef * handle,
	uint8_t port,
	uint8_t pin,
	uint8_t mode
);

void PCA9535_WritePin
(
	PCA9535_HandleTypeDef * handle,
	uint8_t pin
);

uint8_t PCA9535_ReadPin
(
	PCA9535_HandleTypeDef * handle,
	uint8_t pin
);

#ifdef __cplusplus
}
#endif

#endif /* PCA9535_H */
