/*-----------------------------------------------------------------------------
 *      RL-ARM
 *-----------------------------------------------------------------------------
 *      Name:    Serial.h
 *      Purpose: Serial I/O Definitions
 *-----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdint.h>

extern void     SER_Init    (void);
extern int32_t SER_PutChar (int32_t ch);
extern int32_t SER_GetChar (void);
extern int UARTwrite(const char *pcBuf, uint32_t ui32Len);
extern int UARTgets(char *pcBuf, uint32_t ui32Len);

#endif  /* __SERIAL_H */
