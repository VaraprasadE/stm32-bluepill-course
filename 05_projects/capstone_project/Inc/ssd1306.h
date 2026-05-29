#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

#include "tx_api.h"

void ssd1306_init(void);
uint8_t ssd1306_is_ready(void);
UINT ssd1306_render_dashboard(uint16_t value,
                              uint16_t average,
                              uint8_t button,
                              uint8_t alert_active,
                              const uint16_t *ring_buffer,
                              uint8_t ring_size,
                              uint8_t ring_head,
                              uint8_t ring_count);

#endif