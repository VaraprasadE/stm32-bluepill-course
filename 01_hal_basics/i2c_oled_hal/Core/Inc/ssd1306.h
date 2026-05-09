#ifndef __SSD1306_H
#define __SSD1306_H

/**
 * @file    ssd1306.h
 * @brief   SSD1306 OLED display driver API.
 *
 * This header exposes the initialization and drawing functions required to
 * render text to a 128x64 SSD1306 display over I2C.
 */

#include <stdbool.h>
#include <stdint.h>

#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief SSD1306 display width in pixels */
#define SSD1306_WIDTH 128U
/** @brief SSD1306 display height in pixels */
#define SSD1306_HEIGHT 64U

/**
 * @brief  Function pointer type for SSD1306 I2C write callbacks.
 * @param  address 7-bit I2C device address.
 * @param  data Pointer to the transmit buffer.
 * @param  length Number of bytes to transmit.
 * @retval HAL status from the driver.
 */
typedef HAL_StatusTypeDef (*SSD1306_WriteFunction)(uint8_t address, const uint8_t *data, uint16_t length);

/**
 * @brief  Initialize the SSD1306 display driver.
 * @param  write_function I2C write callback used to send data and commands.
 * @retval true when initialization succeeds.
 */
bool SSD1306_Init(SSD1306_WriteFunction write_function);

/**
 * @brief  Query whether the SSD1306 driver is ready.
 * @retval true when the driver has been initialized.
 */
bool SSD1306_IsReady(void);

/**
 * @brief  Clear the internal display buffer.
 */
void SSD1306_Clear(void);

/**
 * @brief  Flush the display buffer to the OLED screen.
 * @retval true on successful transmission.
 */
bool SSD1306_UpdateScreen(void);

/**
 * @brief  Set the text cursor position inside the display buffer.
 * @param  x Horizontal pixel coordinate.
 * @param  y Vertical pixel coordinate.
 */
void SSD1306_SetCursor(uint8_t x, uint8_t y);

/**
 * @brief  Write a null-terminated string into the display buffer.
 * @param  text Pointer to the text string.
 */
void SSD1306_WriteString(const char *text);

#ifdef __cplusplus
}
#endif

#endif /* __SSD1306_H */