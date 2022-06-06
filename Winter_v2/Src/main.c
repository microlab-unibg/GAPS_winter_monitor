/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "app.h"
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
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c3;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_I2C3_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern volatile Winter_t winter;
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
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_I2C3_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
	App_InitSystem();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if (App_GetSystemState() == SYSTEM_STATE_SLEEP) {
			Enter_Stop2_Mode();
			Exit_Stop2_Mode();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_7;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_I2C2|RCC_PERIPHCLK_I2C3;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_SYSCLK;
  PeriphClkInit.I2c3ClockSelection = RCC_I2C3CLKSOURCE_SYSCLK;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x2000090E;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.Timing = 0x2000090E;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MOT_CS_GPIO_Port, MOT_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(EXP_CS_GPIO_Port, EXP_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LOAD_EN_Pin|LED_GREEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_RED_Pin|LED_BLUE_Pin|BLE_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DBG_GPIO1_GPIO_Port, DBG_GPIO1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : MOT_CS_Pin */
  GPIO_InitStruct.Pin = MOT_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MOT_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PH0 PH1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : INT_ACC_Pin BLE_IRQ_Pin */
  GPIO_InitStruct.Pin = INT_ACC_Pin|BLE_IRQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SD_CD_Pin */
  GPIO_InitStruct.Pin = SD_CD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SD_CD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : EXP_CS_Pin */
  GPIO_InitStruct.Pin = EXP_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(EXP_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : EXP_GPIO_Pin */
  GPIO_InitStruct.Pin = EXP_GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(EXP_GPIO_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LOAD_EN_Pin LED_GREEN_Pin */
  GPIO_InitStruct.Pin = LOAD_EN_Pin|LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_RED_Pin LED_BLUE_Pin BLE_RST_Pin */
  GPIO_InitStruct.Pin = LED_RED_Pin|LED_BLUE_Pin|BLE_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : BLE_CS_Pin PB15 PB5 PB8
                           PB9 */
  GPIO_InitStruct.Pin = BLE_CS_Pin|GPIO_PIN_15|GPIO_PIN_5|GPIO_PIN_8
                          |GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : BATT_STAT_Pin DBG_GPIO3_Pin DBG_GPIO2_Pin */
  GPIO_InitStruct.Pin = BATT_STAT_Pin|DBG_GPIO3_Pin|DBG_GPIO2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PC6 PC7 PC8 PC9
                           PC10 PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA9 PA10 PA11
                           PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : DBG_GPIO1_Pin */
  GPIO_InitStruct.Pin = DBG_GPIO1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DBG_GPIO1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

}

/* USER CODE BEGIN 4 */
/**
 * @brief  Write an amount of data in the specified device and register, in
 * 		   blocking mode.
 * @param  i2c        I2C_e enumeration value.
 * @param  DevAddress Target device address.
 * @param  pData 	  Pointer to data buffer.
 * @param  Size 	  Amount of data to be sent.
 * @return HAL status.
 */
uint8_t I2C_Write(I2C_e i2c, uint8_t DevAddress, uint8_t* pData, uint8_t Size) {
	HAL_StatusTypeDef res = HAL_OK;

	switch (i2c) {
		case I2C_ENV:
			res |= HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData, Size,
					100);
			break;
		case I2C_GAS:
			res |= HAL_I2C_Master_Transmit(&hi2c2, DevAddress, pData, Size,
					100);
			break;
		case I2C_EXP:
			res |= HAL_I2C_Master_Transmit(&hi2c3, DevAddress, pData, Size,
					100);
			break;
		default:
			break;
	}

	return res;
}

/**
 * @brief  Read an amount of data from the specified device and register, in
 * 		   blocking mode.
 * @param  i2c        I2C_e enumeration value.
 * @param  DevAddress Target device address.
 * @param  RegAddress Target register address.
 * @param  pData 	  Pointer to data buffer.
 * @param  Size 	  Amount of data to be sent.
 * @return HAL status.
 */
uint8_t I2C_Read(I2C_e i2c, uint8_t DevAddress, uint8_t RegAddress,
		uint8_t* pData, uint16_t Size) {
	HAL_StatusTypeDef res = HAL_OK;

	switch (i2c) {
		case I2C_ENV:
			res |= HAL_I2C_Master_Transmit(&hi2c1, DevAddress, &RegAddress, 1,
					100);
			if (res == HAL_OK)
				res |= HAL_I2C_Master_Receive(&hi2c1, DevAddress, pData, Size,
						100);
			break;
		case I2C_GAS:
			res |= HAL_I2C_Master_Transmit(&hi2c2, DevAddress, &RegAddress, 1,
					100);
			if (res == HAL_OK)
				res |= HAL_I2C_Master_Receive(&hi2c2, DevAddress, pData, Size,
						100);
			break;
		case I2C_EXP:
			res |= HAL_I2C_Master_Transmit(&hi2c3, DevAddress, &RegAddress, 1,
					100);
			if (res == HAL_OK)
				res |= HAL_I2C_Master_Receive(&hi2c3, DevAddress, pData, Size,
						100);
			break;
		default:
			res = HAL_ERROR;
			break;
	}

	return res;
}

/**
 * @brief  Write an amount of data in the specified device, in blocking mode.
 * @param  spi   SPI_e enumeration value.
 * @param  pData Pointer to data buffer.
 * @param  Size  Amount of data to be sent.
 * @return HAL status.
 */
uint8_t SPI_Write(SPI_e spi, uint8_t* pData, uint16_t Size) {
	HAL_StatusTypeDef res = HAL_OK;

	switch (spi) {
		case SPI_BLE:
			break;
		case SPI_MOTION:
			HAL_GPIO_WritePin(MOT_CS_GPIO_Port, MOT_CS_Pin, GPIO_PIN_RESET);
			res |= HAL_SPI_Transmit(&hspi2, pData, Size, 100);
			HAL_GPIO_WritePin(MOT_CS_GPIO_Port, MOT_CS_Pin, GPIO_PIN_SET);
			break;
		case SPI_EXP:
			HAL_GPIO_WritePin(EXP_CS_GPIO_Port, EXP_CS_Pin, GPIO_PIN_RESET);
			res |= HAL_SPI_Transmit(&hspi2, pData, Size, 100);
			HAL_GPIO_WritePin(EXP_CS_GPIO_Port, EXP_CS_Pin, GPIO_PIN_SET);
		default:
			res = HAL_ERROR;
			break;
	}

	return res;
}

/**
 * @brief  Read an amount of data in the specified device, in blocking mode.
 * @param  spi        SPI_e enumeration value.
 * @param  RegAddress Target register address.
 * @param  pData      Pointer to data buffer.
 * @param  Size       Amount of data to be sent.
 * @return HAL status.
 */
uint8_t SPI_Read(SPI_e spi, uint8_t RegAddress, uint8_t* pData, uint16_t Size) {
	HAL_StatusTypeDef res = HAL_OK;

	switch (spi) {
		case SPI_BLE:
			break;
		case SPI_MOTION:
			HAL_GPIO_WritePin(MOT_CS_GPIO_Port, MOT_CS_Pin, GPIO_PIN_RESET);
			pData[0] = 0;
			res |= HAL_SPI_Transmit(&hspi2, &RegAddress, 1, 100);
			res |= HAL_SPI_Receive(&hspi2, pData, Size, 100);
			HAL_GPIO_WritePin(MOT_CS_GPIO_Port, MOT_CS_Pin, GPIO_PIN_SET);
			break;
		case SPI_EXP:
			HAL_GPIO_WritePin(EXP_CS_GPIO_Port, EXP_CS_Pin, GPIO_PIN_RESET);
			res |= HAL_SPI_Transmit(&hspi2, &RegAddress, 1, 100);
			res |= HAL_SPI_Receive(&hspi2, pData, Size, 100);
			HAL_GPIO_WritePin(EXP_CS_GPIO_Port, EXP_CS_Pin, GPIO_PIN_SET);
			break;
		default:
			res = HAL_ERROR;
			break;
	}

	return res;
}

void Enable_LPR_Mode(void) {

	/* *** Low-power run mode ***
	 * ==========================
	 *    [..]
	 *       (+) Entry: (from main run mode)
	 *          (++) set LPR bit with HAL_PWREx_EnableLowPowerRunMode() API
	 *               after having decreased the system clock below 2 MHz.
	 */

	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
//	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

//	/** Adjust divider for SD card interface and re-initialize it
//	 */
//	FRESULT result;
//	result = f_mount(0, SDPath, 1);				// un-mount
//	hsd1.Init.ClockDiv = 0;
//	result |= f_mount(&FatFsDisk, SDPath, 1);	// mount again
//	if (result != FR_OK) {
//		Error_Handler();
//	}

	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_4;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

//	/** Initializes the CPU, AHB and APB busses clocks
//	 */
//	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK; // | RCC_CLOCKTYPE_SYSCLK;
//	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
//	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV8;
//	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
//		Error_Handler();
//	}

//	/** Disable HSI to reduce power consumption
//	 */
//	__HAL_RCC_HSI_DISABLE();

	/** Enter the Low Power Run Mode
	 */
	HAL_PWREx_EnableLowPowerRunMode();

	// Set the LP Mode flag
	winter.lprEnabled = 1;
}

void Disable_LPR_Mode(void) {

	/* *** Low-power run mode ***
	 * ==========================
	 * 	  [..]
	 * 	     (+) Exit:
	 * 	        (++) clear LPR bit then wait for REGLP bit to be reset with
	 * 	        	 HAL_PWREx_DisableLowPowerRunMode() API. Only then can the
	 * 	        	 system clock frequency be increased above 2 MHz.
	 */

	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
//	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Exit the Low Power Run Mode
	 */
	HAL_PWREx_DisableLowPowerRunMode();

//	/** Enable HSI
//	 */
//	__HAL_RCC_HSI_ENABLE();

	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_7;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

//	/** Initializes the CPU, AHB and APB busses clocks
//	 */
//	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK;
//	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
//	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
//		Error_Handler();
//	}

//	/** Re-initialize the SD card interface
//	 */
//	FRESULT result;
//	result = f_mount(0, SDPath, 1);				// un-mount
//	result |= f_mount(&FatFsDisk, SDPath, 1);	// mount again
//	if (result != FR_OK) {
//		Error_Handler();
//	}

// Reset the LP Mode flag
	winter.lprEnabled = 0;
}

void GPIO_Deinit(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* EXTI interrupt deinit*/
	HAL_NVIC_DisableIRQ(EXTI2_IRQn);

	/*Configure GPIO pins : PA1 PA2 PA3 PA4 PA15 */
	GPIO_InitStruct.Pin = SD_CD_Pin | BLE_IRQ_Pin | EXP_CS_Pin | EXP_GPIO_Pin
			| DBG_GPIO1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PB0 PB1 PB2 PB3 PB4 PB11 PB12 */
	GPIO_InitStruct.Pin = LED_RED_Pin | LED_BLUE_Pin | BLE_CS_Pin
			| DBG_GPIO3_Pin | DBG_GPIO2_Pin | BLE_RST_Pin | BATT_STAT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : PC4 PC5 PC8 PC9 PC10 PC11 PC12 PC13 */
	GPIO_InitStruct.Pin = LOAD_EN_Pin | LED_GREEN_Pin | MOT_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : PD2 */
	//GPIO_InitStruct.Pin = SD_CMD_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	// Set SDA and SCL pins of I2C1 e I2C2 to PUSH PULL with HIGH level
	// (alternatively, OPEN DRAIN with PULL-UP)
	/*Configure GPIO pins : PD2 */
	HAL_GPIO_WritePin(GPIOB,
			ENV_SCL_Pin | ENV_SDA_Pin | GAS_SCL_Pin | GAS_SDA_Pin,
			GPIO_PIN_SET);
	GPIO_InitStruct.Pin = ENV_SCL_Pin | ENV_SDA_Pin | GAS_SCL_Pin | GAS_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_DISABLE();
	__HAL_RCC_GPIOB_CLK_DISABLE();
	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOD_CLK_DISABLE();
	__HAL_RCC_GPIOH_CLK_DISABLE();
}

void Enter_Stop2_Mode(void) {
	/*
	 * *** Stop 2 mode ***
	 * ==========================
	 * Stop 2 mode can only be entered from Run mode. It is not
	 * possible to enter Stop 2 mode from the Low-power run mode.
	 */

	// Check the current LPR state
	if (winter.lprEnabled) Disable_LPR_Mode();

	// Deinitialize external peripherals and user-defined IRQs
	App_DeinitPeripherals();
	HAL_NVIC_DisableIRQ(BLE_JOB_IRQn);
	HAL_NVIC_DisableIRQ(JOB_IRQn);

	// Deinitialize internal peripherals
	HAL_I2C_DeInit(&hi2c1);
	HAL_I2C_DeInit(&hi2c2);
	HAL_I2C_DeInit(&hi2c3);
	HAL_SPI_DeInit(&hspi1);

	// Deinitialize the SD card and the related GPIOs
	//FATFS_UnLinkDriver(SDPath);		// equivalent to a MX_FATFS_Deinit() call
	//HAL_SD_DeInit(&hsd1);

	// Disable VCC
	IO_Enable_VCC(GPIO_PIN_RESET);

	// Set to analog all the GPIOs
	GPIO_Deinit();

	// Configure the clock source used after wake up from stop
	__HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);

	// Finally, suspend the SysTick to prevent its interrupts
	HAL_SuspendTick();

	// Enter Stop 2 Mode
	HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
}

void Exit_Stop2_Mode(void) {
	// First, re-configure the system clock
	SystemClock_Config();

	// Resume the SysTick
	HAL_ResumeTick();

	// Re-initialize internal peripherals
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_I2C2_Init();
	MX_I2C3_Init();
	//MX_SDMMC1_SD_Init();
	MX_SPI1_Init();
	MX_SPI2_Init();
	//MX_FATFS_Init();

	//TODO Check the SD card since it wasn't working at this point
	//f_mount(&FatFsDisk, SDPath, 1);

	// Finally, re-initialize the system
	//TODO Maybe ->IDLE? Then, explicitly resume the LPR mode and BLE_Init()
	App_InitSystem();
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
	// Suspend tick
	HAL_SuspendTick();

	// Turn ON red LED
	IO_ResetLED(LED_GREEN | LED_BLUE);
	IO_SetLED(LED_RED);

	while (1) {
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
	 tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
