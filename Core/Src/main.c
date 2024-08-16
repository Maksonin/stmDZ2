/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t status = 1;
uint8_t letGo = 0;
uint8_t seconds = 0;
uint8_t delay = 0;
uint8_t pData[8] = {0};
uint8_t dataReceived=0; // признак данное получено
uint8_t dataTransmitted=1; // признак данное передано

enum numColor {
	CAR_GREEN,
	CAR_YELLOW,
	CAR_RED,
	PEOPLE_GREEN,
	PEOPLE_RED
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
void setIntBufer(int32_t num, uint8_t buf[], uint8_t len);
void setStatus(void);
void setStrob(enum numColor numColor, uint8_t numRepeat);
void setAllRed(void);
void setCarRed(void);
void setCarYellow(void);
void setCarGreen(void);
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
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_UART_Receive_IT (&huart1, pData, 8);
  while (1)
  {
//	  uint8_t state = HAL_UART_GetState(&huart1);
//	  if( (state != HAL_UART_STATE_BUSY_RX) && (state != HAL_UART_STATE_BUSY_TX_RX) ) {
//
//	    while( HAL_UART_Transmit_IT(&huart1, pData, 8) == HAL_BUSY );


	  setStatus();

	  if(seconds < delay){
		  if(letGo == 1){
			  if((delay - seconds) > 5){
				  status = 2;
				  letGo = 0;
				  continue;
			  } else {
				  letGo = 0;
			  }
		  }
	  }
	  else {
		  seconds = 0;
		  delay = 0;
		  status++;
	  }

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
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
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 7999;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, carGreen_Pin|carYellow_Pin|peopleGreen_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, carRed_Pin|peopleRed_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : btn_Pin */
  GPIO_InitStruct.Pin = btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : carGreen_Pin carYellow_Pin carRed_Pin peopleGreen_Pin
                           peopleRed_Pin */
  GPIO_InitStruct.Pin = carGreen_Pin|carYellow_Pin|carRed_Pin|peopleGreen_Pin
                          |peopleRed_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
 * Функция формирует массив buf длиной len из разрядов числа num (тип uint32)
 * Массив записывается с конца
 * Нулевой элемент массива buf используется для знака
 * */
void setIntBufer(int32_t num, uint8_t buf[], uint8_t len){
	  len -= 1;

	  if(num < 0){ // если переданное в функцию число отрицательное
		  num *= -1; // делаем число положительным
		  buf[0] = '-'; // записываем знак в нулевой элемент массива
	  }
	  else
		  buf[0] = 0; // иначе обнуляем

	  if(num == 0){ // если в функцию был передан 0
		  buf[len] = '0';
	  }
	  else { // иначе производим перебор переданного числа с записью каждого разряда в элемент массива buf
		  while(num){
			  buf[len] = '0' + (num % 10);
			  num /= 10;
			  len--;
		  }
	  }
}


// Функция вызывается из прерывания таймера (каждую 1 секунду)
void timerIT(){
	uint8_t str[] = "Timer!\r\n";
	HAL_UART_Transmit(&huart1, str, 8, 1);

	uint8_t strSec[] = "Sec - ";
	uint8_t strDel[] = " Delay - ";
	uint8_t strStatus[] = " Status - ";
	uint8_t strNum[8];

	HAL_UART_Transmit(&huart1, strSec, 6, 1);
	setIntBufer(seconds, strNum, 2);
	HAL_UART_Transmit(&huart1, strNum, 2, 1);

	HAL_UART_Transmit(&huart1, strDel, 9, 1);
	setIntBufer(delay, strNum, 2);
	HAL_UART_Transmit(&huart1, strNum, 2, 1);

	HAL_UART_Transmit(&huart1, strStatus, 10, 1);
	setIntBufer(status, strNum, 2);
	HAL_UART_Transmit(&huart1, strNum, 2, 1);

	uint8_t strN[] = "\r\n";
	HAL_UART_Transmit(&huart1, strN, 2, 1);

	seconds++;
}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//
//  if(huart == &huart1) {
//
//    dataReceived=1;
//
//    if( dataTransmitted != 0 ) {
//		HAL_UART_Transmit_IT(&huart1, pData, 8);
//		dataReceived=0;
//		dataTransmitted=0;
//    }
//
//    HAL_UART_Receive_IT (&huart1, pData, 8);
//  }
//}
//
//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
//
//  if(huart == &huart1) {
//
//    dataTransmitted=1;
//
//    if( dataReceived != 0 ) {
//      HAL_UART_Transmit_IT(&huart1, pData, 8);
//      dataReceived=0;
//      dataTransmitted=0;
//    }
//  }
//}

// Функция вызывается из прерывания UART
void uartIT(){
	if( pData[0] == '1' ){
		status = 2;
		delay = 0;
	}

	HAL_UART_Receive_IT (&huart1, pData, 8);
}

// Функция вызывается из прерывания GPIO
void peopleBtnClick(){
	uint8_t str[] = "Click!\r\n";
	HAL_UART_Transmit(&huart1, str, 8, 1);
	if(status == 1){
		delay = 0;
		letGo = 1;
	}
}

// функция устанавливает текущее состояние светофоров в соответствии с переменной status
void setStatus(){
	if(status > 9)
		status = 1;

	switch (status){
		case 0:
			break;
		case 1:
			delay = 15;
			setCarGreen();
			break;
		case 2:
			setStrob(CAR_GREEN, 5);
			break;
		case 3:
			delay = 3;
			setCarYellow();
			break;
		case 4:
			delay = 3;
			setAllRed();
			break;
		case 5:
			delay = 6;
			setCarRed();
			break;
		case 6:
			setStrob(PEOPLE_GREEN, 5);
			break;
		case 7:
			delay = 3;
			setAllRed();
			break;
		case 8:
			delay = 3;
			HAL_GPIO_WritePin(GPIOB, carYellow_Pin, GPIO_PIN_SET);
			break;
		case 9:
			break;
	}
}

// Все сигналы красные
void setAllRed(){
	HAL_GPIO_WritePin(GPIOB, carGreen_Pin|carYellow_Pin|peopleGreen_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, carRed_Pin|peopleRed_Pin, GPIO_PIN_SET);
}

// Пешеходам зеленый, автомобилям красный
void setCarRed(){
	HAL_GPIO_WritePin(GPIOB, carGreen_Pin|carYellow_Pin|peopleRed_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, carRed_Pin|peopleGreen_Pin, GPIO_PIN_SET);
}

// Пешеходам красный, автомобилям желтый
void setCarYellow(){
	HAL_GPIO_WritePin(GPIOB, carGreen_Pin|carRed_Pin|peopleGreen_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, carYellow_Pin|peopleRed_Pin, GPIO_PIN_SET);
}

// Пешеходам красный, автомобилям зеленый
void setCarGreen(){
	HAL_GPIO_WritePin(GPIOB, carYellow_Pin|carRed_Pin|peopleGreen_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, carGreen_Pin|peopleRed_Pin, GPIO_PIN_SET);
}

// Функция выполняет моргание цветом (enum numColor) количеством равным numRepeat
void setStrob(enum numColor numColor, uint8_t numRepeat){
	while((numRepeat * 2) > 0){
		seconds = 0;
		while(seconds < 1){}
		switch (numColor){
			case CAR_GREEN:
				HAL_GPIO_TogglePin(carGreen_GPIO_Port, carGreen_Pin);
				break;
			case CAR_YELLOW:
				HAL_GPIO_TogglePin(carYellow_GPIO_Port, carYellow_Pin);
				break;
			case CAR_RED:
				HAL_GPIO_TogglePin(carRed_GPIO_Port, carRed_Pin);
				break;
			case PEOPLE_GREEN:
				HAL_GPIO_TogglePin(peopleGreen_GPIO_Port, peopleGreen_Pin);
				break;
			case PEOPLE_RED:
				HAL_GPIO_TogglePin(peopleRed_GPIO_Port, peopleRed_Pin);
				break;
		}

		numRepeat--;
	}
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
