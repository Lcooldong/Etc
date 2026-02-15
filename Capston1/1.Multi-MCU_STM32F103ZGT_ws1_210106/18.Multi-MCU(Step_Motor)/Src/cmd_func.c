#include <stdio.h>                      /* Standard I/O .h-file               */
#include <ctype.h>                      /* Character functions                */
#include <string.h>                     /* String and memory functions        */


#include "cmdline.h"
#include "cmd_func.h"
#include "main.h"


/* Command function prototypes */
int cmd_help    (int argc, char *argv[]);
int cmd_on       (int argc, char *argv[]);
int cmd_off       (int argc, char *argv[]);
int cmd_inv      (int argc, char *argv[]);
int cmd_tim3_period  (int argc, char *argv[]);
int cmd_pwm  (int argc, char *argv[]);
int cmd_period  (int argc, char *argv[]);
int cmd_stepm  (int argc, char *argv[]);


extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;



//*****************************************************************************
//
// A temporary data buffer used when manipulating file paths, or reading data
// from the SD card.
//
//*****************************************************************************
char g_pcTmpBuf[PATH_BUF_SIZE];

//*****************************************************************************
//
// The buffer that holds the command line.
//
//*****************************************************************************
char g_pcCmdBuf[CMD_BUF_SIZE];


//*****************************************************************************
//
// This is the table that holds the command names, implementing functions, and
// brief description.
//
//*****************************************************************************
tCmdLineEntry g_psCmdTable[] =
{
    { "help",     cmd_help,     "Display list of commands" },
    { "h",        cmd_help,     "alias for help" },
    { "?",        cmd_help,     "alias for help" },
    { "on",       cmd_on,       "LED ON" },		
		{ "off",      cmd_off,      "LED OFF" },
		{ "inv",      cmd_inv,      "LED Toggle" },
	  { "tim3_period",      cmd_tim3_period,      "Tim3 Preiod" },
		{ "pwm",      cmd_pwm,      "PWM Duty Ratio" },
		{ "period",   cmd_period,      "PWM Period" },		
		{ "stepm",    cmd_stepm,      "Setp Motor" },	
    { 0, 0, 0 }
};


unsigned long
atoul(char *str)
{
	unsigned long ret = 0;
	int multi;
	char c;

	if (!strncmp(str, "0X", 2) || !strncmp(str, "0x", 2))
	{
		str += 2;	// skip "0x"
		multi = 16;
	} else
		multi = 10;

	while (c = *str++)
	{
		if (c >= '0' && c <= '9')
			ret = ret * multi + c - '0';
		else if (c >= 'a' && c <= 'f')
			ret = ret * multi + c - 'a' + 10;
		else if (c >= 'A' && c <= 'F')
			ret = ret * multi + c - 'A' + 10;
		else
			return ret;
	}
	return ret;
}



int cmd_inv      (int argc, char *argv[]){
	
  unsigned char led_num = 0;
	
	
	if(argc != 2 )
  {
     printf(" inv  [num]\n\r");
     return 0;
  }	
	
	led_num = (uint8_t)(atoul(argv[1]));
	
	GPIOF->ODR =  GPIOF->ODR  ^ (1 << led_num);

  //
  // Return success.
  //
  return(0);	
	
};



int cmd_on(int argc, char *argv[]) {

  unsigned char led_num = 0;
	
	
	if(argc != 2 )
  {
     printf(" on  [num]\n\r");
     return 0;
  }
	
	led_num = (uint8_t)(atoul(argv[1]));
		
  GPIOF->ODR =  GPIOF->ODR  | (1 << led_num);
	
  //
  // Return success.
  //
  return(0);	
	
}


int cmd_off(int argc, char *argv[]) {

  unsigned char led_num = 0;
	
	
	
	if(argc != 2 )
  {
     printf(" off  [num]\n\r");
     return 0;
  }
	
	led_num = (uint8_t)(atoul(argv[1]));
		
  GPIOF->ODR =  GPIOF->ODR  & ~(1 << led_num);
	
  //
  // Return success.
  //
  return(0);	
	
}


int cmd_tim3_period(int argc, char *argv[]) {

  uint16_t period_num = 0;
	
	
	
	if(argc != 2 )
  {
     printf(" preiod  [num]\n\r");
     return 0;
  }
	
	period_num = (uint16_t)(atoul(argv[1]));
		
  TIM3->ARR = period_num;
	
  //
  // Return success.
  //
  return(0);	
	
}

int cmd_pwm(int argc, char *argv[]) {

  uint8_t ch_num = 0;
	uint16_t duty_num = 0;
	
	
	
	if(argc != 3 )
  {
     printf(" pwm [ch] [duty ratio]\n\r");
     return 0;
  }
	
	ch_num = (uint16_t)(atoul(argv[1]));
	duty_num = (uint16_t)(atoul(argv[2]));
	
	duty_num = (__HAL_TIM_GET_AUTORELOAD(&htim5) * duty_num )/ 1000;
	printf("PWM CH[%d], Period[%d], Compare Value[%d]\n\r",ch_num, __HAL_TIM_GET_AUTORELOAD(&htim5), duty_num);
	
	if(ch_num == 1){		
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, duty_num);
	}
	else if(ch_num == 2){
		__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, duty_num);
	}
	else if(ch_num == 3){
		__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, duty_num);	
	}
	
  //
  // Return success.
  //
  return(0);	
	
}


int cmd_period(int argc, char *argv[]) {

	uint16_t period_num = 0;
	
	
	
	if(argc != 2 )
  {
     printf(" period [period]\n\r");
     return 0;
  }
	
	period_num = (uint16_t)(atoul(argv[1]));

	
	printf("New Period[%d], Old Period[%d]\n\r",period_num, __HAL_TIM_GET_AUTORELOAD(&htim5));
	
	
  __HAL_TIM_SET_AUTORELOAD(&htim5, period_num);

	
  //
  // Return success.
  //
  return(0);	
	
}


int cmd_stepm(int argc, char *argv[]) {

  uint16_t step_num = 0;
	volatile uint16_t temp = 0;
	uint8_t dir = 0;
	
	if(argc != 3 )
  {
     printf(" stepm [num] [dir(0 or 1)]\n\r");
     return 0;
  }
	
	step_num = (uint16_t)(atoul(argv[1]));
	dir = (uint8_t)(atoul)(argv[2]);
	
	
  for(int i = 0 ; i < step_num*2; i++)
	{
		GPIOA->ODR= GPIOA->ODR ^ (0x01 << 2);	// PA2
	  if(dir == 0){
			//HAL_GPIO_WritePin(GPIOG, GPIO_PIN_11, GPIO_PIN_RESET);  // Function	(Easy to Understand)
			GPIOG->ODR = GPIOG->ODR & (~(0x01 << 11));	// RESET -> to 0		// Register (Speed up)
		}else if(dir == 1){
			//HAL_GPIO_WritePin(GPIOG, GPIO_PIN_11, GPIO_PIN_SET);		// Function
			GPIOG->ODR = GPIOG->ODR | (0x01 << 11);			// SET   -> to 1		// direct Register control
		}
		for(temp = 0 ; temp < 5000; temp++);

	}
	
  //
  // Return success.
  //
  return(0);	
	
}

/*-----------------------------------------------------------------------------
 *        Display Command Syntax help
 *----------------------------------------------------------------------------*/
//*****************************************************************************
//
// This function implements the "help" command.  It prints a simple list of the
// available commands with a brief description.
//
//*****************************************************************************
int cmd_help(int argc, char *argv[]){
	
    tCmdLineEntry *psEntry;

    //
    // Print some header text.
    //
    printf("\nAvailable commands\n\r");
    printf("------------------\n\r");

    //
    // Point at the beginning of the command table.
    //
    psEntry = &g_psCmdTable[0];

    //
    // Enter a loop to read each entry from the command table.  The end of the
    // table has been reached when the command name is NULL.
    //
    while(psEntry->pcCmd)
    {
        //
        // Print the command name and the brief description.
        //
        printf("%6s: %s\n\r", psEntry->pcCmd, psEntry->pcHelp);

        //
        // Advance to the next entry in the table.
        //
        psEntry++;
    }

    //
    // Return success.
    //
    return(0);
}
