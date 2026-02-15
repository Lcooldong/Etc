/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/*
 * Dot matrix controller firmawre
 *
 * <Schematic Domain>
 *
 * DOT_DATA_M[0:2] => TXS0108EPWR (Level Shift) => DOT_DATA_S[0:2]
 *
 * DOT_DATA_S[0:2] => MC74HC138ADT (Demultiplexer) => DOT_DATA[0:7]
 *
 * DOT_DATA[0:7]	=> 74HC574 (D Flip-Flop) => 74HC04PW (Inverter) => COL[0:4]
 * 								=> 74HC574 (D Flip-Flop) => 74HC04PW (Inverter) => ULN2803ADWR (Darlington Transitor) => ROW[0:6]
 *
 * <Required knowledge>
 *
 * - Multiplexer Decoder (MC74HC138ADT)
 * - D Flip-Flop (74HC574)
 * - Dynamic LED Control
 *
 * <구현 순서>
 *
 * - Multiplexer Decoder (MC74HC138ADT) Datasheet
 * https://www.alldatasheet.com/datasheet-pdf/pdf/12162/ONSEMI/MC74HC138ADT.html
 *
 * - D Flip-Flop (74HC574)
 * https://www.alldatasheet.com/datasheet-pdf/pdf/15641/PHILIPS/74HC574.html
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
  	for (int i = 0; i < 7; i++) {
    	HAL_GPIO_WritePin(GPIOE, DOT_DATA0_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOE, DOT_DATA1_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOE, DOT_DATA2_Pin, GPIO_PIN_RESET);

    	if (i & 1) {
    		HAL_GPIO_WritePin(GPIOE, DOT_DATA0_Pin, GPIO_PIN_SET);
    	}
    	if (i & 2) {
    		HAL_GPIO_WritePin(GPIOE, DOT_DATA1_Pin, GPIO_PIN_SET);
    	}
    	if (i & 4) {
    		HAL_GPIO_WritePin(GPIOE, DOT_DATA2_Pin, GPIO_PIN_SET);
    	}


    	HAL_GPIO_WritePin(GPIOE, DOT_CS1_Pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(GPIOE, DOT_CS1_Pin, GPIO_PIN_RESET);

  		for (int k = 0; k < 5; k++) {
      	HAL_GPIO_WritePin(GPIOE, DOT_DATA0_Pin, GPIO_PIN_RESET);
      	HAL_GPIO_WritePin(GPIOE, DOT_DATA1_Pin, GPIO_PIN_RESET);
      	HAL_GPIO_WritePin(GPIOE, DOT_DATA2_Pin, GPIO_PIN_RESET);
        
      	if (k & 1) {
      		HAL_GPIO_WritePin(GPIOE, DOT_DATA0_Pin, GPIO_PIN_SET);
      	}
      	if (k & 2) {
      		HAL_GPIO_WritePin(GPIOE, DOT_DATA1_Pin, GPIO_PIN_SET);
      	}
      	if (k & 4) {
      		HAL_GPIO_WritePin(GPIOE, DOT_DATA2_Pin, GPIO_PIN_SET);
      	}

  	  	HAL_GPIO_WritePin(GPIOE, DOT_CS0_Pin, GPIO_PIN_SET);
  	  	HAL_GPIO_WritePin(GPIOE, DOT_CS0_Pin, GPIO_PIN_RESET);
  		}
  	}

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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, DOT_DATA0_Pin|DOT_DATA1_Pin|DOT_DATA2_Pin|DOT_CS0_Pin 
                          |DOT_CS1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DOT_DATA0_Pin DOT_DATA1_Pin DOT_DATA2_Pin DOT_CS0_Pin 
                           DOT_CS1_Pin */
  GPIO_InitStruct.Pin = DOT_DATA0_Pin|DOT_DATA1_Pin|DOT_DATA2_Pin|DOT_CS0_Pin 
                          |DOT_CS1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
