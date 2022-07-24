/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
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
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_TIM6_Init(void);
static void MX_GPIO_Init(void);
static void MX_TIM7_Init(void);
/* USER CODE BEGIN PFP */
void _tim_timeout_blocking(uint32_t time);
void _tim_timeout_nonblocking_with_callback(uint32_t time, void (*_tim_callbackmethod)(TIM_HandleTypeDef *htim), uint8_t with_time_refresh);
void _userfunc(TIM_HandleTypeDef *htim);
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
  MX_TIM6_Init();
  MX_GPIO_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(RGB_BL_GPIO_Port, RGB_BL_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(RGB_RT_GPIO_Port, RGB_RT_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(RGB_GN_GPIO_Port, RGB_GN_Pin, GPIO_PIN_SET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // rote LED einschalten
	  HAL_GPIO_WritePin(RGB_RT_GPIO_Port, RGB_RT_Pin, GPIO_PIN_RESET);
	  for(uint64_t i=0; i<40000;i++);

	  // Aufruf der nicht-blockierenden-Funktion mit der Timer-Ablaufzeit für den Benutzerfunktionsaufruf, der Benutzerfunktion und einer "Aktualisierungsoption der Timer-Ablaufzeit" als Übergabeparameter
      _tim_timeout_nonblocking_with_callback(5000, _userfunc,0);
      for(uint64_t i=0; i<20000;i++);

      // Aufruf der blockierenden-Funktion mit der "Blockier-Zeit" als Übergabeparameter
      _tim_timeout_blocking(3000);


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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_4;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 1000-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 1000-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OnePulse_Init(&htim6, TIM_OPMODE_SINGLE) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 1000-1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 1000-1;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OnePulse_Init(&htim7, TIM_OPMODE_SINGLE) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RGB_BL_Pin|RGB_RT_Pin|RGB_GN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RGB_BL_Pin RGB_RT_Pin RGB_GN_Pin */
  GPIO_InitStruct.Pin = RGB_BL_Pin|RGB_RT_Pin|RGB_GN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void _tim_timeout_blocking(uint32_t time)
{
	// rote LED ausschalten
	HAL_GPIO_WritePin(RGB_RT_GPIO_Port, RGB_RT_Pin, GPIO_PIN_SET);

	// Timer6 stoppen
	HAL_TIM_Base_Stop_IT(&htim6);

	// Blockingzeit (Timer6) setzen
	__HAL_TIM_SET_AUTORELOAD(&htim6,time);

	// Aktualisierungs-Interrupt-Flag (Timer6) löschen
	__HAL_TIM_CLEAR_FLAG(&htim6,TIM_FLAG_UPDATE);

	// Update-Interrupt (Timer7) ausschalten
	__HAL_TIM_DISABLE_IT(&htim7,TIM_IT_UPDATE);

	// Timer6 starten
	HAL_TIM_Base_Start_IT(&htim6);

	// SysTick-Interrupt ausschalten
	HAL_SuspendTick();

	// in den SleepMode wechseln -> Aufwachen nach Interrupt
  	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

  	// SysTick-Interrupt einschalten
	HAL_ResumeTick();

	// Update-Interrupt (Timer7) einschalten
	__HAL_TIM_ENABLE_IT(&htim7,TIM_IT_UPDATE);
}

void _tim_timeout_nonblocking_with_callback(uint32_t time, void (*_tim_callbackmethod)(TIM_HandleTypeDef *htim), uint8_t with_time_refresh)
{
	// Zeitaktualisierung beim Wiedereintritt -> 0-Nein / 1-Ja
	if(!READ_BIT(TIM7->CR1,TIM_CR1_CEN_Msk) || with_time_refresh )
	{
		// Timer7 stoppen
		HAL_TIM_Base_Stop_IT(&htim7);

		// Timer7 Period_Elapsed-Callbackmethode registrieren
		HAL_TIM_RegisterCallback(&htim7, HAL_TIM_PERIOD_ELAPSED_CB_ID,_tim_callbackmethod);

		// Zeit für Timer7-Interrupt setzen
		__HAL_TIM_SET_AUTORELOAD(&htim7,time);

		// Aktualisierungs-Interrupt-Flag (Timer7) löschen
		__HAL_TIM_CLEAR_FLAG(&htim7,TIM_FLAG_UPDATE);

		// Timer7 starten
		HAL_TIM_Base_Start_IT(&htim7);
	}
}

// Benutzer-Funktion - grün blinkende LED
void _userfunc(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_WritePin(RGB_GN_GPIO_Port, RGB_GN_Pin, GPIO_PIN_RESET);
	for(int i=0; i<10000;i++);
	HAL_GPIO_WritePin(RGB_GN_GPIO_Port, RGB_GN_Pin, GPIO_PIN_SET);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
