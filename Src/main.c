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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <locale.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MPU6050_ADDR 0x68<<1
#define PWM_MGMT_1_REG 0x6B
#define SMPLRT_DIV_REG 0x19
#define GYRO_CNFG_REG 0x1B
#define ACC_CNFG_REG 0x1C
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
uint8_t data;
uint8_t buffer[2],tuffer[6],cuffer[6];
int16_t gyro_raw[3], acc_raw[3];
float gyro_cal[3];
int16_t acc_total_vector;
float angle_pitch_gyro;
float angle_roll_gyro;
float angle_pitch_acc;
float angle_roll_acc;
float angle_pitch;
float angle_roll;
int16_t raw_temp;
float temp;
int i;
float prevtime,prevtime1,time1, elapsedtime1,prevtime2,time2,elapsedtime2;
char ax[20],ay[20],az[20];

HAL_StatusTypeDef set_gyro;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
#include "LCD.h"
#include "stdio.h"
#include "string.h"
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  lcd_init(_LCD_4BIT, _LCD_FONT_5x8, _LCD_2LINE);
  lcd_clear();
  data = 0x00;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, PWM_MGMT_1_REG , 1, &data, 1, HAL_MAX_DELAY);
    data = 0x08;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, GYRO_CNFG_REG , 1, &data, 1, HAL_MAX_DELAY);
    data = 0x10;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, ACC_CNFG_REG , 1, &data, 1, HAL_MAX_DELAY);
    for(i=0;i<2000;i++){
  	  prevtime2=time2;
  	  time2= HAL_GetTick();
  	  elapsedtime2=(time2-prevtime2)*1000;
  	  cuffer[0]=0x43;
  	  HAL_I2C_Master_Transmit(&hi2c1, MPU6050_ADDR, cuffer, 1, HAL_MAX_DELAY);
  	  HAL_I2C_Master_Receive(&hi2c1, MPU6050_ADDR, cuffer, 6, HAL_MAX_DELAY);

  	  gyro_raw[0]=(cuffer[0]<<8|cuffer[1]);
  	  gyro_raw[1]=(cuffer[2]<<8|cuffer[3]);
  	  gyro_raw[2]=(cuffer[4]<<8|cuffer[5]);

  	  gyro_cal[0] += gyro_raw[0];
  	  gyro_cal[1] += gyro_raw[1];
  	  gyro_cal[2] += gyro_raw[2];

  	  HAL_Delay(3);

    }

    gyro_cal[0]/=2000;
    gyro_cal[1]/=2000;
    gyro_cal[2]/=2000;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  prevtime1=time1;
	  	time1=HAL_GetTick();
	  	elapsedtime1=(time1-prevtime1)*1000;

	  	tuffer[0]=0x3B;
	  	HAL_I2C_Master_Transmit(&hi2c1, MPU6050_ADDR, tuffer, 1, HAL_MAX_DELAY);
	  	HAL_I2C_Master_Receive(&hi2c1, MPU6050_ADDR, tuffer, 6, HAL_MAX_DELAY);

	  	acc_raw[0] = (tuffer[0]<<8|tuffer[1]);
	  	acc_raw[1] = (tuffer[2]<<8|tuffer[3]);
	  	acc_raw[2] = (tuffer[4]<<8|tuffer[5]);

	  	buffer[0]=0x41;
	  	HAL_I2C_Master_Transmit(&hi2c1, MPU6050_ADDR, buffer, 1, HAL_MAX_DELAY);
	  	HAL_I2C_Master_Receive(&hi2c1, MPU6050_ADDR, buffer, 6, HAL_MAX_DELAY);

	  	raw_temp=(buffer[0]<<8|buffer[1]);
	  	temp=(raw_temp/340.0)+36.53;

	  	cuffer[0]=0x43;
	  	HAL_I2C_Master_Transmit(&hi2c1, MPU6050_ADDR, cuffer, 1, HAL_MAX_DELAY);
	  	HAL_I2C_Master_Receive(&hi2c1, MPU6050_ADDR, cuffer, 6, HAL_MAX_DELAY);

	  	gyro_raw[0]= (cuffer[0]<<8|cuffer[1]);
	  	gyro_raw[1]= (cuffer[2]<<8|cuffer[3]);
	  	gyro_raw[2]= (cuffer[4]<<8|cuffer[5]);

	  	gyro_raw[0] -= gyro_cal[0];
	  	gyro_raw[1] -= gyro_cal[1];
	  	gyro_raw[1] -= gyro_cal[2];

	  	angle_pitch_gyro += gyro_raw[0]* 0.0000611;
	  	angle_roll_gyro += gyro_raw[1] * 0.0000611;

	  	angle_pitch_gyro += angle_roll_gyro * sin(gyro_raw[2]*0.000001066);
	  	angle_roll_gyro -= angle_pitch_gyro * sin(gyro_raw[2]*0.000001066);

	  	acc_total_vector= sqrt((acc_raw[0]*acc_raw[0])+(acc_raw[1]*acc_raw[1])+(acc_raw[2]*acc_raw[2]));

	  	angle_pitch_acc = asin((float)acc_raw[1]/acc_total_vector)*57.296;
	  	angle_roll_acc = asin((float)acc_raw[0]/acc_total_vector)* -57.296;

	  	angle_pitch_acc -= 0.05; //0.05
	  	angle_roll_acc -= -1.32; //-1.32

	  	if(set_gyro){
	  		angle_pitch=angle_pitch_gyro*0.9996+angle_pitch_acc*0.0004;
	  		angle_roll=angle_roll_gyro*0.9996+angle_roll_acc*0.0004;

	  	}
	  	else{
	  		angle_pitch=angle_pitch_acc;
	  		set_gyro=true;
	  	}
	  	sprintf(ax, "%d", acc_raw[0]);
	  	sprintf(ay, "%d", acc_raw[1]);
	  	sprintf(az, "%d", acc_raw[2]);

	  	lcd_print(0, 0, "x:");
	  	lcd_print(0, 2, ax);
	  /*	lcd_print(0, 6, "y:");
	  	lcd_print(0, 8, ay);
	  	lcd_print(0, 12, "z");
	  	lcd_print(0, 13, az);*/
	  	HAL_Delay(300);
	  	lcd_clear();

	  	while((HAL_GetTick()-prevtime)*1000<5000);
	  	prevtime=HAL_GetTick();
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
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
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_EN_Pin|LCD_RS_Pin|LCD_D4_Pin|LCD_D5_Pin
                          |LCD_D6_Pin|LCD_D7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LCD_EN_Pin LCD_RS_Pin LCD_D4_Pin LCD_D5_Pin
                           LCD_D6_Pin LCD_D7_Pin */
  GPIO_InitStruct.Pin = LCD_EN_Pin|LCD_RS_Pin|LCD_D4_Pin|LCD_D5_Pin
                          |LCD_D6_Pin|LCD_D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
