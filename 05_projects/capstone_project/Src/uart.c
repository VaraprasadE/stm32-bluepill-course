#include <stdint.h>

#include "capstone_platform.h"
#include "uart.h"

void usart1_init(void)
{
  RCC_APB2ENR |= (RCC_IOPAEN | RCC_USART1EN);

  GPIOA_CRH &= ~((0xFUL << GPIO_CRH_MODE9_Pos) | (0xFUL << GPIO_CRH_MODE10_Pos));
  GPIOA_CRH |= (0xBUL << GPIO_CRH_MODE9_Pos);
  GPIOA_CRH |= (0x4UL << GPIO_CRH_MODE10_Pos);

  USART1_BRR = 0x0341U;
  USART1_CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

void usart1_send_char(char c)
{
  uint32_t timeout = 100000U;

  while (((USART1_SR & USART_SR_TXE) == 0U) && (timeout > 0U))
  {
    timeout--;
  }

  if (timeout == 0U)
  {
    return;
  }

  USART1_DR = (uint32_t)(uint8_t)c;
}

void usart1_send_string(const char *str)
{
  while (*str != '\0')
  {
    usart1_send_char(*str++);
  }
}

char usart1_receive_char(void)
{
  while ((USART1_SR & USART_SR_RXNE) == 0U)
  {
  }

  return (char)(USART1_DR & 0xFFU);
}

UINT usart1_data_available(void)
{
  return ((USART1_SR & USART_SR_RXNE) != 0U) ? TX_TRUE : TX_FALSE;
}

void usart1_send_uint32(uint32_t value)
{
  char buffer[10];
  uint32_t index = 0U;

  if (value == 0U)
  {
    usart1_send_char('0');
    return;
  }

  while ((value > 0U) && (index < (uint32_t)sizeof(buffer)))
  {
    buffer[index++] = (char)('0' + (value % 10U));
    value /= 10U;
  }

  while (index > 0U)
  {
    usart1_send_char(buffer[--index]);
  }
}