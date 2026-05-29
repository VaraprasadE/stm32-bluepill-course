#ifndef I2C_H
#define I2C_H

#include <stdint.h>

#include "tx_api.h"

void i2c1_init(void);
UINT i2c1_write_bytes(uint8_t address, uint8_t control_byte, const uint8_t *data, uint16_t length);

#endif