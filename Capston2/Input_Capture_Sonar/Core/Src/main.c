/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint16_t risingCapture[2];
volatile uint16_t fallingCapture[2];
uint8_t risingCNT = 0;
uint8_t fallingCNT = 0;
uint32_t period, freq, duty;
uint16_t width, distance;



struct __FILE {
    int dummy;
};
FILE __stdout;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF); 

  return ch;
}
uint32_t DWT_Delay_Init(void);
void DWT_Delay_us(volatile uint32_t microseconds);
void TriggerEnable(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	// timer, channel, array, quantity 
	DWT_Delay_Init();
	// HAL_TIM_PeriodElapsedCallback
	HAL_TIM_Base_Start_IT(&htim4);	// Sonar
	
	// HAL_TIM_OC_DelayElapsedCallback
  HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_1);		// Sonar		
	
	// HAL_TIM_IC_CaptureCallback
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);		
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);

	printf("start the program\r\n");
	HAL_Delay(2000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

		HAL_Delay(100);
			
		printf("[%5d, %5d]-%5d = %5d => Distance %4d cm| period : %6d | freq : %3d Hz| duty : %d %%\r\n", risingCapture[0], risingCapture[1], fallingCapture[0], width ,distance, period , freq, duty);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM4)
	{
		GPIOB->ODR |= 0x01 << 6;  // PB 6 SET
	}
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM4)
	{
		GPIOB->ODR &= ~(0x01 << 6);	// PB 6	RESET, Pulse
	}
}



void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	/* Rising */
	if(htim->Instance == TIM3 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{

		risingCapture[risingCNT] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		if(risingCNT == 1){
			risingCNT = 0;
			if(risingCapture[0] > risingCapture[1])
			{
				period = TIM3->ARR - risingCapture[0] + risingCapture[1];
				GPIOB->ODR |= LD3_Pin;
			}
			else
			{
				period = risingCapture[1] - risingCapture[0];
			}
			
			period += TIM3->ARR;	// 65535 + 34000 =>  sonar PSC 150 rising PSC 90
		}
		else
		{
			risingCNT = 1;
			GPIOB->ODR &= ~LD3_Pin;
		}
		GPIOB->ODR |= LD1_Pin;
		GPIOB->ODR &= ~LD2_Pin;
		
		freq = (HAL_RCC_GetPCLK1Freq()*2)/(htim3.Instance->PSC + 1);	// (45MHz*2) / 90 = 1MHz = 1,000,000 Hz
		freq = freq/period;
		

	}
	
	/* Falling */
	if(htim->Instance == TIM3 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		fallingCapture[fallingCNT] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		if(fallingCNT == 1){
			fallingCNT = 0;
		}else{
			fallingCNT = 1;
		}
		
		if(fallingCapture[0] >= risingCapture[0] && fallingCapture[0] <= risingCapture[1])
		{
			width = fallingCapture[0] - risingCapture[0];
			
		}
		else if(fallingCapture[1] >= risingCapture[0] && fallingCapture[1] <= risingCapture[1])
		{
			width = fallingCapture[1] - risingCapture[0];
		}
		GPIOB->ODR &= ~LD1_Pin;
		GPIOB->ODR |= LD2_Pin;

		distance = width / 58;
		if(distance > 400) distance = 400;
		
		duty = width * 100/period;
	
	}
}


uint32_t DWT_Delay_Init(void)
{
  /* Disable TRC */
  CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
  /* Enable TRC */
  CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;

  /* Disable clock cycle counter */
  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
  /* Enable  clock cycle counter */
  DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;

  /* Reset the clock cycle counter value */
  DWT->CYCCNT = 0;

     /* 3 NO OPERATION instructions */
  __ASM volatile ("NOP");
  __ASM volatile ("NOP");
  __ASM volatile ("NOP");

  /* Check if clock cycle counter has started */
  if(DWT->CYCCNT)
  {
		return 0; /*clock cycle counter started*/
  }
		else
  {
    return 1; /*clock cycle counter not started*/
  }
}

void DWT_Delay_us(volatile uint32_t microseconds)
{
  uint32_t clk_cycle_start = DWT->CYCCNT;

  /* Go to number of cycles for system */
  microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);

  /* Delay till end */
  while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}

void TriggerEnable(void)
{
	GPIOB->ODR &= ~TRIGGER_Pin;	// initialize
	DWT_Delay_us(2);	
	GPIOB->ODR |= TRIGGER_Pin;	// Trigger signal
	DWT_Delay_us(11);
	GPIOB->ODR &= ~TRIGGER_Pin;	// shut down
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

