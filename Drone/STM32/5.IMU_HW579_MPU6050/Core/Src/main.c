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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "HW579.h"
#include "MPU6050.h"
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
HW579 hw579;
AxisData compassData;
AccelData accelData;
GyroData gyroData;

MPU6050 mpu6050;
ANGLE angle;

extern uint8_t uart3_rx_flag;
extern uint8_t uart3_rx_data;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
int _write(uint32_t file, uint8_t* ptr, uint32_t len )
{

	for(int i = 0; i < len; i++)
	{
		LL_USART_TransmitData8(USART3, *(ptr+i));
		HAL_Delay(1);
	}

	return len;

	//if(HAL_UART_Transmit(&huart3, ptr, len, len) == HAL_OK) return len;
	//else return 0;
}
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
  MX_USART3_UART_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  HW579_Init(&hw579, I2C1, 1.3);
  MPU6050_Init(&mpu6050, I2C2);

  LL_TIM_EnableCounter(TIM3);
  LL_USART_EnableIT_RXNE(USART3);	// Receive Complete Interrupt -> IRQ Handler

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
  for(int i=0; i< 4; i++)
   	  {
   		  TIM3->PSC = 3000 - 500*i;
   		  //printf("TIM3->PSC : %ld\r\n", TIM3->PSC);
   		  HAL_Delay(100);

   	  }
   LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);


//  float f = 1.234;
//  int32_t fToInt = (int32_t)(f * 1000);
//  printf("%f %ld\r\n", f, fToInt);

//  float a = 1.3f;
//  if(a==1.3f)printf("a float\r\n");
//  else printf("a float not working\r\n");

  Calibrate_Gyro(hw579.GYRO, &gyroData, 1000, 2);
  MPU_Gyrocali(&mpu6050, &angle);
  MPU_Angcali(&mpu6050, &angle);


  printf("%f %f %f\r\n", gyroData.base_gyro_X, gyroData.base_gyro_Y, gyroData.base_gyro_Z);
  uint8_t debug_buffer[256];

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  Read_Magneto((hw579.MAGNETO), &compassData);
	  Read_Accel(hw579.ACCEL, &accelData);
	  Read_Gyro(hw579.GYRO, &gyroData);
	  Read_Gyro_Temperature(hw579.GYRO, &gyroData);
	  sprintf((char*)debug_buffer, "MAGNETO_RAW : %5d %5d %5d | ACCEL_RAW   : %5d %5d %5d | GYRO_RAW    : %5d %5d %5d | %4.2f",
			  compassData.XAxis, compassData.YAxis, compassData.ZAxis,
			  accelData.raw_accel_X, accelData.raw_accel_Y, accelData.raw_accel_Z,
			  gyroData.raw_gyro_X, gyroData.raw_gyro_Y, gyroData.raw_gyro_Z, gyroData.gyro_Temp);
	  //printf("%s\r\n", debug_buffer);

	  MPU_Readaccgyro(&mpu6050, &angle);
	  //printf("%ld %ld %ld\r\n", angle.getmpuaccx, angle.getmpuaccy, angle.getmpuaccz);



	  //printf("%lf\r\n", (double)fToInt/1000);
	  //fToInt += 1;
	  HAL_Delay(500);


	  if(uart3_rx_flag == 1)
	  {
		  uart3_rx_flag = 0;
		  LL_USART_TransmitData8(USART3, uart3_rx_data);	// Transmit received data
		  switch(uart3_rx_data)
		  {
		  	  case '0':
		  		  LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_0| LL_GPIO_PIN_7|LL_GPIO_PIN_14);
		  		  HAL_Delay(500);
		  		  break;

		  	  case '1': LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4); break;
		  	  case '2': LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4); break;
		  }
	  }


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
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
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
