#ifndef CAPSTONE_PLATFORM_H
#define CAPSTONE_PLATFORM_H

#include <stdint.h>

#define SYSTICK_CTRL                 (*(volatile uint32_t *)0xE000E010UL)
#define SYSTICK_LOAD                 (*(volatile uint32_t *)0xE000E014UL)
#define SYSTICK_VAL                  (*(volatile uint32_t *)0xE000E018UL)
#define SCB_SHPR3                    (*(volatile uint32_t *)0xE000ED20UL)

#define RCC_APB2ENR                  (*(volatile uint32_t *)0x40021018UL)
#define RCC_APB1ENR                  (*(volatile uint32_t *)0x4002101CUL)
#define RCC_CFGR                     (*(volatile uint32_t *)0x40021004UL)

#define GPIOA_CRL                    (*(volatile uint32_t *)0x40010800UL)
#define GPIOA_CRH                    (*(volatile uint32_t *)0x40010804UL)
#define GPIOA_ODR                    (*(volatile uint32_t *)0x4001080CUL)
#define GPIOA_IDR                    (*(volatile uint32_t *)0x40010808UL)

#define GPIOB_CRL                    (*(volatile uint32_t *)0x40010C00UL)
#define GPIOB_ODR                    (*(volatile uint32_t *)0x40010C0CUL)

#define GPIOC_CRH                    (*(volatile uint32_t *)0x40011004UL)
#define GPIOC_ODR                    (*(volatile uint32_t *)0x4001100CUL)
#define GPIOC_BSRR                   (*(volatile uint32_t *)0x40011010UL)

#define USART1_SR                    (*(volatile uint32_t *)0x40013800UL)
#define USART1_DR                    (*(volatile uint32_t *)0x40013804UL)
#define USART1_BRR                   (*(volatile uint32_t *)0x40013808UL)
#define USART1_CR1                   (*(volatile uint32_t *)0x4001380CUL)

#define I2C1_CR1                     (*(volatile uint32_t *)0x40005400UL)
#define I2C1_CR2                     (*(volatile uint32_t *)0x40005404UL)
#define I2C1_DR                      (*(volatile uint32_t *)0x40005410UL)
#define I2C1_SR1                     (*(volatile uint32_t *)0x40005414UL)
#define I2C1_SR2                     (*(volatile uint32_t *)0x40005418UL)
#define I2C1_CCR                     (*(volatile uint32_t *)0x4000541CUL)
#define I2C1_TRISE                   (*(volatile uint32_t *)0x40005420UL)

#define ADC1_SR                      (*(volatile uint32_t *)0x40012400UL)
#define ADC1_CR1                     (*(volatile uint32_t *)0x40012404UL)
#define ADC1_CR2                     (*(volatile uint32_t *)0x40012408UL)
#define ADC1_SMPR2                   (*(volatile uint32_t *)0x40012410UL)
#define ADC1_SQR3                    (*(volatile uint32_t *)0x40012434UL)
#define ADC1_DR                      (*(volatile uint32_t *)0x4001244CUL)

#define RCC_IOPAEN                   (1UL << 2)
#define RCC_IOPBEN                   (1UL << 3)
#define RCC_IOPCEN                   (1UL << 4)
#define RCC_ADC1EN                   (1UL << 9)
#define RCC_USART1EN                 (1UL << 14)
#define RCC_I2C1EN                   (1UL << 21)

#define GPIO_CRH_MODE13_Pos          20U
#define GPIO_CRH_CNF13_Pos           22U
#define GPIO_CRH_MODE13_MASK         (3UL << GPIO_CRH_MODE13_Pos)
#define GPIO_CRH_CNF13_MASK          (3UL << GPIO_CRH_CNF13_Pos)
#define GPIO_ODR_PIN13               (1UL << 13)
#define GPIO_BSRR_BS13               (1UL << 13)

#define GPIO_CRH_MODE9_Pos           4U
#define GPIO_CRH_CNF9_Pos            6U
#define GPIO_CRH_MODE10_Pos          8U
#define GPIO_CRH_CNF10_Pos           10U

#define SYSTICK_CTRL_ENABLE          (1UL << 0)
#define SYSTICK_CTRL_TICKINT         (1UL << 1)
#define SYSTICK_CTRL_CLKSOURCE       (1UL << 2)
#define SCB_SHPR3_PENDSV_LOWEST      (0xFFUL << 16)
#define SCB_SHPR3_SYSTICK_LOWEST     (0xFFUL << 24)

#define USART_CR1_UE                 (1UL << 13)
#define USART_CR1_TE                 (1UL << 3)
#define USART_CR1_RE                 (1UL << 2)
#define USART_SR_TXE                 (1UL << 7)
#define USART_SR_RXNE                (1UL << 5)

#define I2C_CR1_PE                   (1UL << 0)
#define I2C_CR1_START                (1UL << 8)
#define I2C_CR1_STOP                 (1UL << 9)
#define I2C_CR1_SWRST                (1UL << 15)
#define I2C_SR1_SB                   (1UL << 0)
#define I2C_SR1_ADDR                 (1UL << 1)
#define I2C_SR1_BTF                  (1UL << 2)
#define I2C_SR1_TXE                  (1UL << 7)
#define I2C_SR1_AF                   (1UL << 10)
#define I2C_SR2_BUSY                 (1UL << 1)

#define ADC_CR2_ADON                 (1UL << 0)
#define ADC_CR2_CONT                 (1UL << 1)
#define ADC_CR2_CAL                  (1UL << 2)
#define ADC_CR2_RSTCAL               (1UL << 3)
#define ADC_CR2_EXTTRIG              (1UL << 20)
#define ADC_CR2_SWSTART              (1UL << 22)
#define ADC_CR2_EXTSEL_SWSTART       (7UL << 17)
#define ADC_SR_EOC                   (1UL << 1)

static inline void delay_cycles(volatile uint32_t count)
{
  while (count-- != 0U)
  {
    __asm volatile ("nop");
  }
}

#endif