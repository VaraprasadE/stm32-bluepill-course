/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f103xb.h"
#include "ssd1306.h"


#define SYSTEM_CLOCK_HZ 8000000U
#define APB1_CLOCK_HZ SYSTEM_CLOCK_HZ
#define APB2_CLOCK_HZ SYSTEM_CLOCK_HZ
#define UART_BAUDRATE 9600U
#define I2C_TIMEOUT_MS 20U
#define LDR_UPDATE_INTERVAL_MS 300U


static GPIO_PinState led_state = GPIO_PIN_SET;
static GPIO_PinState previous_button_state = GPIO_PIN_SET;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);

static uint16_t LDR_ReadRaw(void);
static void UART1_WriteString(const char *text);
static void I2C1_Reset(void);
static HAL_StatusTypeDef I2C1_MasterWrite(uint8_t address, const uint8_t *data, uint16_t length);


static HAL_StatusTypeDef I2C1_WaitFlagSet(volatile uint32_t *reg, uint32_t mask)
{
  uint32_t start_tick = HAL_GetTick();

  while (((*reg) & mask) == 0U)
  {
    if ((I2C1->SR1 & I2C_SR1_AF) != 0U)
    {
      I2C1->SR1 &= ~I2C_SR1_AF;
      return HAL_ERROR;
    }

    if ((HAL_GetTick() - start_tick) > I2C_TIMEOUT_MS)
    {
      return HAL_TIMEOUT;
    }
  }

  return HAL_OK;
}

static HAL_StatusTypeDef I2C1_MasterWrite(uint8_t address, const uint8_t *data, uint16_t length)
{
  uint16_t index;

  I2C1->CR1 |= I2C_CR1_START;
  if (I2C1_WaitFlagSet(&I2C1->SR1, I2C_SR1_SB) != HAL_OK)
  {
    I2C1_Reset();
    return HAL_ERROR;
  }

  I2C1->DR = (uint32_t)(address << 1U);
  if (I2C1_WaitFlagSet(&I2C1->SR1, I2C_SR1_ADDR) != HAL_OK)
  {
    I2C1->CR1 |= I2C_CR1_STOP;
    I2C1_Reset();
    return HAL_ERROR;
  }

  (void)I2C1->SR1;
  (void)I2C1->SR2;

  for (index = 0U; index < length; index++)
  {
    if (I2C1_WaitFlagSet(&I2C1->SR1, I2C_SR1_TXE) != HAL_OK)
    {
      I2C1->CR1 |= I2C_CR1_STOP;
      I2C1_Reset();
      return HAL_ERROR;
    }

    I2C1->DR = data[index];
  }

  if (I2C1_WaitFlagSet(&I2C1->SR1, I2C_SR1_BTF) != HAL_OK)
  {
    I2C1->CR1 |= I2C_CR1_STOP;
    I2C1_Reset();
    return HAL_ERROR;
  }

  I2C1->CR1 |= I2C_CR1_STOP;
  return HAL_OK;
}

static void I2C1_Reset(void)
{
  I2C1->CR1 &= ~I2C_CR1_PE;
  I2C1->CR1 = 0U;
  I2C1->CR2 = (APB1_CLOCK_HZ / 1000000U);
  I2C1->CCR = (uint16_t)(APB1_CLOCK_HZ / (100000U * 2U));
  I2C1->TRISE = (uint16_t)((APB1_CLOCK_HZ / 1000000U) + 1U);
  I2C1->CR1 = I2C_CR1_PE;
}

static uint16_t LDR_ReadRaw(void)
{
  ADC1->SQR3 = 0U;
  ADC1->SR = 0U;
  ADC1->CR2 |= ADC_CR2_SWSTART;

  while ((ADC1->SR & ADC_SR_EOC) == 0U)
  {
  }

  return (uint16_t)(ADC1->DR & 0x0FFFU);
}

static void UART1_WriteString(const char *text)
{
  size_t index;
  size_t length = strlen(text);

  for (index = 0U; index < length; index++)
  {
    while ((USART1->SR & USART_SR_TXE) == 0U)
    {
    }

    USART1->DR = (uint8_t)text[index];
  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  SystemClock_Config();
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();

  (void)SSD1306_Init(I2C1_MasterWrite);
  UART1_WriteString("System ready\r\n");
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, led_state);



  /* Infinite loop */
  while (1)
  {

    char oled_line[22];
    char uart_line[48];
    uint16_t ldr_raw = LDR_ReadRaw();
    uint32_t ldr_mv = ((uint32_t)ldr_raw * 3300U) / 4095U;
    GPIO_PinState button_state = HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin);

    if ((previous_button_state == GPIO_PIN_SET) && (button_state == GPIO_PIN_RESET))
    {
      led_state = (led_state == GPIO_PIN_SET) ? GPIO_PIN_RESET : GPIO_PIN_SET;
      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, led_state);
    }
    previous_button_state = button_state;

    snprintf(uart_line, sizeof(uart_line), "LDR=%u (%lu.%03luV)\r\n",
             ldr_raw,
             ldr_mv / 1000U,
             ldr_mv % 1000U);
    UART1_WriteString(uart_line);

    if (SSD1306_IsReady())
    {
      SSD1306_Clear();

      SSD1306_SetCursor(0U, 0U);
      snprintf(oled_line, sizeof(oled_line), "LDR:%4u", ldr_raw);
      SSD1306_WriteString(oled_line);

      SSD1306_SetCursor(0U, 24U);
      snprintf(oled_line, sizeof(oled_line), "V:%lu.%03lu",
               ldr_mv / 1000U,
               ldr_mv % 1000U);
      SSD1306_WriteString(oled_line);

      if (!SSD1306_UpdateScreen())
      {
        UART1_WriteString("OLED update failed, retrying\r\n");
        I2C1_Reset();
        (void)SSD1306_Init(I2C1_MasterWrite);
      }
    }

    HAL_Delay(LDR_UPDATE_INTERVAL_MS);
  }
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

/* USER CODE BEGIN 4 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);

  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LDR_ADC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LDR_ADC_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = I2C1_SCL_Pin | I2C1_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = SPI1_NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SPI1_NSS_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = SPI1_SCK_Pin | SPI1_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = SPI1_MISO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SPI1_MISO_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = UART1_TX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(UART1_TX_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = UART1_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(UART1_RX_GPIO_Port, &GPIO_InitStruct);
}

static void MX_ADC1_Init(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_ADCPRE) | RCC_CFGR_ADCPRE_DIV2;

  ADC1->CR1 = 0U;
  ADC1->CR2 = 0U;
  ADC1->SMPR1 = 0U;
  ADC1->SMPR2 = ADC_SMPR2_SMP0;
  ADC1->SQR1 = 0U;
  ADC1->SQR2 = 0U;
  ADC1->SQR3 = 0U;

  ADC1->CR2 |= ADC_CR2_ADON;
  HAL_Delay(1);
  ADC1->CR2 |= ADC_CR2_RSTCAL;
  while ((ADC1->CR2 & ADC_CR2_RSTCAL) != 0U)
  {
  }

  ADC1->CR2 |= ADC_CR2_CAL;
  while ((ADC1->CR2 & ADC_CR2_CAL) != 0U)
  {
  }

  ADC1->CR2 |= ADC_CR2_EXTTRIG | ADC_CR2_EXTSEL;
}

static void MX_I2C1_Init(void)
{
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

  I2C1->CR1 = 0U;
  I2C1->CR2 = (APB1_CLOCK_HZ / 1000000U);
  I2C1->CCR = (uint16_t)(APB1_CLOCK_HZ / (100000U * 2U));
  I2C1->TRISE = (uint16_t)((APB1_CLOCK_HZ / 1000000U) + 1U);
  I2C1->CR1 = I2C_CR1_PE;
}

static void MX_SPI1_Init(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

  SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_BR_1;
  SPI1->CR2 = 0U;
  SPI1->CR1 |= SPI_CR1_SPE;
}

static void MX_USART1_UART_Init(void)
{
  uint32_t usartdiv_x16 = (APB2_CLOCK_HZ + (UART_BAUDRATE / 2U)) / UART_BAUDRATE;

  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

  USART1->CR1 = 0U;
  USART1->BRR = ((usartdiv_x16 / 16U) << 4U) | (usartdiv_x16 % 16U);
  USART1->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
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
#ifdef USE_FULL_ASSERT
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
