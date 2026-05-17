/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Bare-metal USART1 communication for STM32F103C8T6
 ******************************************************************************
 */

#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

// ----------------------------------------------------------------------------
// 1. Memory Map and Register Addresses [1]
// ----------------------------------------------------------------------------

// Reset and Clock Control (RCC)
#define RCC_APB2ENR   (*(volatile uint32_t *)0x40021018U)

// GPIO Port A
#define GPIOA_CRH     (*(volatile uint32_t *)0x40010804U)

// USART1 Registers
#define USART1_SR     (*(volatile uint32_t *)0x40013800U)
#define USART1_DR     (*(volatile uint32_t *)0x40013804U)
#define USART1_BRR    (*(volatile uint32_t *)0x40013808U)
#define USART1_CR1    (*(volatile uint32_t *)0x4001380CU)

// ----------------------------------------------------------------------------
// 2. Bit Mask Definitions
// ----------------------------------------------------------------------------

// RCC APB2ENR bits [2, 3]
#define RCC_IOPAEN    (1U << 2)   // GPIOA clock enable
#define RCC_USART1EN  (1U << 14)  // USART1 clock enable

// GPIOA_CRH Configuration bit positions for PA9 (TX) and PA10 (RX) [4]
#define GPIO_CRH_MODE9_Pos  4U
#define GPIO_CRH_CNF9_Pos   6U
#define GPIO_CRH_MODE10_Pos 8U
#define GPIO_CRH_CNF10_Pos  10U

// USART1_CR1 bits [5-7]
#define USART_CR1_UE  (1U << 13) // USART Enable
#define USART_CR1_TE  (1U << 3)  // Transmitter Enable
#define USART_CR1_RE  (1U << 2)  // Receiver Enable

// USART1_SR bits [8]
#define USART_SR_TXE  (1U << 7)  // Transmit Data Register Empty
#define USART_SR_RXNE (1U << 5)  // Read Data Register Not Empty

// ----------------------------------------------------------------------------
// 3. Helper Functions
// ----------------------------------------------------------------------------

void USART1_SendChar(char c)
{
  // Wait until the Transmit Data Register Empty (TXE) flag is set to 1 [8]
  while (!(USART1_SR & USART_SR_TXE));

  // Write the character to the Data Register (DR) to be transmitted [9]
  USART1_DR = (c & 0xFF);
}

void USART1_SendString(const char *str)
{
  while (*str)
  {
    USART1_SendChar(*str++);
  }
}


char USART1_ReceiveChar(void)
{
  // Wait until the Receive Data Register Not Empty (RXNE) flag is set to 1
  while (!(USART1_SR & USART_SR_RXNE));

  // Read the character from the Data Register (DR). This read automatically clears the RXNE flag.
  return (char)(USART1_DR & 0xFF);
}

static int USART1_DataAvailable(void)
{
  return (USART1_SR & USART_SR_RXNE) != 0U;
}

// ----------------------------------------------------------------------------
// 4. Main Program
// ----------------------------------------------------------------------------

int main(void)
{
  // Step A: Enable the peripheral clocks for GPIOA and USART1 [2, 3, 10]
  RCC_APB2ENR |= (RCC_IOPAEN | RCC_USART1EN);

  // Step B: Configure the GPIO pins (PA9 for TX, PA10 for RX)
  // First, clear the MODE and CNF bits for PA9 and PA10 [4]
  GPIOA_CRH &= ~((0xFU << GPIO_CRH_MODE9_Pos) | (0xFU << GPIO_CRH_MODE10_Pos));

  // Configure PA9 (USART1_TX) as Alternate Function Push-Pull, max speed 50 MHz [11, 12]
  // CNF = 10, MODE = 11 -> 1011 in binary (0xB)
  GPIOA_CRH |= (0xBU << GPIO_CRH_MODE9_Pos);

  // Configure PA10 (USART1_RX) as Input Floating [12, 13]
  // CNF = 01, MODE = 00 -> 0100 in binary (0x4)
  GPIOA_CRH |= (0x4U << GPIO_CRH_MODE10_Pos);

  // Step C: Set the Baud Rate [9, 14]
  // Assuming the system is running at the default 8 MHz (HSI).
  // Desired Baud Rate: 9600
  // USARTDIV = 8,000,000 / (16 * 9600) = 52.0833
  // Mantissa = 52 (0x34)
  // Fraction = 16 * 0.0833 = 1.33 (rounded to 1 -> 0x1)
  // Register value: 0x341
  USART1_BRR = 0x0341U;

  // Step D: Enable the USART, Transmitter, and Receiver [5-7]
  USART1_CR1 = (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);

  for (;;)
  {
    // Transmit "Hello" every second
    USART1_SendString("Hello\r\n");
// instead of stuck here
    // Wait approximately 1 second while still polling for incoming bytes
    volatile uint32_t count = 1000000U;
    while (count-- != 0U)
    {
      if (USART1_DataAvailable())
      {
        char received_char = USART1_ReceiveChar();
        USART1_SendString("Received: ");
        USART1_SendChar(received_char);
        USART1_SendString("\r\n");  //CR+LF
      }
    }
  }
}
