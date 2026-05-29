#include <stdint.h>

#include "adc.h"
#include "capstone_platform.h"

void adc_init(void)
{
  RCC_APB2ENR |= (RCC_IOPAEN | RCC_ADC1EN);

  RCC_CFGR &= ~(3UL << 14);
  RCC_CFGR |= (2UL << 14);

  GPIOA_CRL &= ~(0xFUL << 0);

  ADC1_SMPR2 &= ~(7UL << 0);
  ADC1_SMPR2 |= (7UL << 0);
  ADC1_SQR3 = 0U;

  ADC1_CR1 = 0U;
  ADC1_CR2 = ADC_CR2_EXTSEL_SWSTART | ADC_CR2_EXTTRIG | ADC_CR2_CONT;
  ADC1_CR2 |= ADC_CR2_ADON;

  delay_cycles(10000U);

  ADC1_CR2 |= ADC_CR2_RSTCAL;
  while ((ADC1_CR2 & ADC_CR2_RSTCAL) != 0U)
  {
  }

  ADC1_CR2 |= ADC_CR2_CAL;
  while ((ADC1_CR2 & ADC_CR2_CAL) != 0U)
  {
  }

  ADC1_CR2 |= ADC_CR2_ADON;
  ADC1_CR2 |= ADC_CR2_SWSTART;
}

uint16_t adc_read_sample(void)
{
  uint32_t timeout = 100000U;

  while (((ADC1_SR & ADC_SR_EOC) == 0U) && (timeout > 0U))
  {
    timeout--;
  }

  if (timeout == 0U)
  {
    return 0U;
  }

  return (uint16_t)(ADC1_DR & 0x0FFFU);
}