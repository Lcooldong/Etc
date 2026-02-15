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

#include <drivers/drv_pca9535.h>

void PCA9535_Init (PCA9535_HandleTypeDef * handle, I2C_HandleTypeDef * i2c_handle, uint8_t addr)
{
	handle->handle = i2c_handle;
	handle->addr = addr;
	handle->portData = 0x00;
	handle->outData = 0x00;
	handle->tmpData[0] = 0x00;

	// TODO :: GPIO set low at initial
}

void PCA9535_WriteData(PCA9535_HandleTypeDef * handle, uint8_t cmd, uint8_t data)
{
	handle->tmpData[0] = data;
	HAL_I2C_Mem_Write(handle->handle, handle->addr, cmd, 1, handle->tmpData, 1, 100);
}

void PCA9535_PinMode(PCA9535_HandleTypeDef * handle, uint8_t port, uint8_t pin, uint8_t mode)
{
	if (mode == PCA9535_INPUT) {
		handle->outData |= pin;
	} else if (mode == PCA9535_OUTPUT) {
		handle->outData &= ~pin;
	}

	PCA9535_WriteData(handle, (port + 0x05), handle->portData);
}
