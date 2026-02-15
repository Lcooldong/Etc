/*-----------------------------------------------------------------------------
 *      RL-ARM
 *-----------------------------------------------------------------------------
 *      Name:    cmd_func.h
 *      Purpose: UART Console Definitions
 *-----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef __CMD_FUNC_H
#define __CMD_FUNC_H

#include <stdint.h>
#include "cmdline.h"

/* Command function prototypes */
extern int cmd_help    (int argc, char *argv[]);
extern int cmd_rmdir (int argc, char *argv[]);
extern int cmd_mkdir (int argc, char *argv[]);
extern int cmd_format (int argc, char *argv[]);
extern int cmd_cdr(int argc, char *argv[]);
extern int cmd_cd(int argc, char *argv[]);
extern int cmd_ls (int argc, char *argv[]);
extern int cmd_time_set(int argc, char *argv[]);
extern int cmd_time_get(int argc, char *argv[]);


#define PATH_BUF_SIZE           80
#define CMD_BUF_SIZE            64

#define M0 0    //SD Card
#define N0  1    //FLASH Memory


extern char g_pcCwdBuf[PATH_BUF_SIZE];
extern char g_pcN0CwdBuf[PATH_BUF_SIZE];
extern char g_pcM0CwdBuf[PATH_BUF_SIZE];


extern char g_pcTmpBuf[PATH_BUF_SIZE];
extern char g_pcCmdBuf[CMD_BUF_SIZE];
extern uint8_t g_u8Drv;
extern tCmdLineEntry g_psCmdTable[];


#endif  /* CMD_FUNC_H */





