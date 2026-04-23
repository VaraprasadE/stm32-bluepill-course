#ifndef __SSD1306_H
#define __SSD1306_H

#include <stdbool.h>
#include <stdint.h>

#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SSD1306_WIDTH 128U
#define SSD1306_HEIGHT 64U

typedef HAL_StatusTypeDef (*SSD1306_WriteFunction)(uint8_t address, const uint8_t *data, uint16_t length);

bool SSD1306_Init(SSD1306_WriteFunction write_function);
bool SSD1306_IsReady(void);
void SSD1306_Clear(void);
bool SSD1306_UpdateScreen(void);
void SSD1306_SetCursor(uint8_t x, uint8_t y);
void SSD1306_WriteString(const char *text);

#ifdef __cplusplus
}
#endif

#endif /* __SSD1306_H */