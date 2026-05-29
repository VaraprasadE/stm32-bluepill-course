#include <stdint.h>

#include "capstone_platform.h"
#include "i2c.h"

#define I2C_TIMEOUT_COUNT            100000U

static UINT wait_for_flag(volatile uint32_t *reg, uint32_t mask)
{
  uint32_t timeout = I2C_TIMEOUT_COUNT;

  while (((*reg & mask) == 0U) && (timeout > 0U))
  {
    timeout--;
  }

  return (timeout > 0U) ? TX_SUCCESS : TX_NOT_AVAILABLE;
}

static void i2c1_stop(void)
{
  I2C1_CR1 |= I2C_CR1_STOP;
}

static UINT i2c1_start(void)
{
  uint32_t timeout = I2C_TIMEOUT_COUNT;

  while (((I2C1_SR2 & I2C_SR2_BUSY) != 0U) && (timeout > 0U))
  {
    timeout--;
  }

  if (timeout == 0U)
  {
    return TX_NOT_AVAILABLE;
  }

  I2C1_CR1 |= I2C_CR1_START;

  return wait_for_flag(&I2C1_SR1, I2C_SR1_SB);
}

static UINT i2c1_send_address(uint8_t address)
{
  volatile uint32_t temp;
  uint32_t timeout = I2C_TIMEOUT_COUNT;

  I2C1_DR = (uint32_t)(address << 1);

  while (((I2C1_SR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0U) && (timeout > 0U))
  {
    timeout--;
  }

  if (timeout == 0U)
  {
    return TX_NOT_AVAILABLE;
  }

  if ((I2C1_SR1 & I2C_SR1_AF) != 0U)
  {
    I2C1_SR1 &= ~I2C_SR1_AF;
    i2c1_stop();
    return TX_NOT_AVAILABLE;
  }

  temp = I2C1_SR1;
  temp = I2C1_SR2;
  (void)temp;

  return TX_SUCCESS;
}

static UINT i2c1_write_data(uint8_t data)
{
  if (wait_for_flag(&I2C1_SR1, I2C_SR1_TXE) != TX_SUCCESS)
  {
    return TX_NOT_AVAILABLE;
  }

  I2C1_DR = data;

  if (wait_for_flag(&I2C1_SR1, I2C_SR1_BTF) != TX_SUCCESS)
  {
    return TX_NOT_AVAILABLE;
  }

  return TX_SUCCESS;
}

void i2c1_init(void)
{
  RCC_APB2ENR |= RCC_IOPBEN;
  RCC_APB1ENR |= RCC_I2C1EN;

  GPIOB_CRL &= ~(0xFFUL << 24);
  GPIOB_CRL |= (0xFFUL << 24);
  GPIOB_ODR |= (1UL << 6) | (1UL << 7);

  I2C1_CR1 |= I2C_CR1_SWRST;
  I2C1_CR1 &= ~I2C_CR1_SWRST;

  I2C1_CR2 = 8U;
  I2C1_CCR = 40U;
  I2C1_TRISE = 9U;
  I2C1_CR1 |= I2C_CR1_PE;
}

UINT i2c1_write_bytes(uint8_t address, uint8_t control_byte, const uint8_t *data, uint16_t length)
{
  uint16_t index;

  if (i2c1_start() != TX_SUCCESS)
  {
    return TX_NOT_AVAILABLE;
  }

  if (i2c1_send_address(address) != TX_SUCCESS)
  {
    i2c1_stop();
    return TX_NOT_AVAILABLE;
  }

  if (i2c1_write_data(control_byte) != TX_SUCCESS)
  {
    i2c1_stop();
    return TX_NOT_AVAILABLE;
  }

  for (index = 0U; index < length; index++)
  {
    if (i2c1_write_data(data[index]) != TX_SUCCESS)
    {
      i2c1_stop();
      return TX_NOT_AVAILABLE;
    }
  }

  i2c1_stop();

  return TX_SUCCESS;
}