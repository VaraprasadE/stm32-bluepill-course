#include "ssd1306.h"

#include <string.h>

#define SSD1306_PRIMARY_ADDRESS 0x3CU
#define SSD1306_SECONDARY_ADDRESS 0x3DU
#define SSD1306_PAGES (SSD1306_HEIGHT / 8U)
#define SSD1306_BUFFER_SIZE (SSD1306_WIDTH * SSD1306_PAGES)
#define SSD1306_TEXT_SCALE 2U

static const uint8_t ssd1306_font[][5] = {
  [0] = {0x00U, 0x00U, 0x00U, 0x00U, 0x00U},
  [' '] = {0x00U, 0x00U, 0x00U, 0x00U, 0x00U},
  ['.'] = {0x00U, 0x60U, 0x60U, 0x00U, 0x00U},
  [':'] = {0x00U, 0x36U, 0x36U, 0x00U, 0x00U},
  ['0'] = {0x3EU, 0x51U, 0x49U, 0x45U, 0x3EU},
  ['1'] = {0x00U, 0x42U, 0x7FU, 0x40U, 0x00U},
  ['2'] = {0x42U, 0x61U, 0x51U, 0x49U, 0x46U},
  ['3'] = {0x21U, 0x41U, 0x45U, 0x4BU, 0x31U},
  ['4'] = {0x18U, 0x14U, 0x12U, 0x7FU, 0x10U},
  ['5'] = {0x27U, 0x45U, 0x45U, 0x45U, 0x39U},
  ['6'] = {0x3CU, 0x4AU, 0x49U, 0x49U, 0x30U},
  ['7'] = {0x01U, 0x71U, 0x09U, 0x05U, 0x03U},
  ['8'] = {0x36U, 0x49U, 0x49U, 0x49U, 0x36U},
  ['9'] = {0x06U, 0x49U, 0x49U, 0x29U, 0x1EU},
  ['A'] = {0x7EU, 0x11U, 0x11U, 0x11U, 0x7EU},
  ['D'] = {0x7FU, 0x41U, 0x41U, 0x22U, 0x1CU},
  ['L'] = {0x7FU, 0x40U, 0x40U, 0x40U, 0x40U},
  ['R'] = {0x7FU, 0x09U, 0x19U, 0x29U, 0x46U},
  ['S'] = {0x46U, 0x49U, 0x49U, 0x49U, 0x31U},
  ['T'] = {0x01U, 0x01U, 0x7FU, 0x01U, 0x01U},
  ['V'] = {0x1FU, 0x20U, 0x40U, 0x20U, 0x1FU}
};

static SSD1306_WriteFunction ssd1306_write = NULL;
static bool ssd1306_ready = false;
static uint8_t ssd1306_address = SSD1306_PRIMARY_ADDRESS;
static uint8_t ssd1306_buffer[SSD1306_BUFFER_SIZE];
static uint8_t ssd1306_cursor_x = 0U;
static uint8_t ssd1306_cursor_y = 0U;

static HAL_StatusTypeDef SSD1306_WriteCommand(uint8_t command)
{
  uint8_t packet[2] = {0x00U, command};

  if (ssd1306_write == NULL)
  {
    return HAL_ERROR;
  }

  return ssd1306_write(ssd1306_address, packet, 2U);
}

static HAL_StatusTypeDef SSD1306_WriteData(const uint8_t *data, uint16_t length)
{
  uint8_t packet[17];
  uint16_t offset = 0U;

  if (ssd1306_write == NULL)
  {
    return HAL_ERROR;
  }

  packet[0] = 0x40U;
  while (offset < length)
  {
    uint16_t chunk = length - offset;

    if (chunk > 16U)
    {
      chunk = 16U;
    }

    memcpy(&packet[1], &data[offset], chunk);
    if (ssd1306_write(ssd1306_address, packet, (uint16_t)(chunk + 1U)) != HAL_OK)
    {
      return HAL_ERROR;
    }

    offset += chunk;
  }

  return HAL_OK;
}

static void SSD1306_ClearBuffer(void)
{
  memset(ssd1306_buffer, 0, sizeof(ssd1306_buffer));
}

static void SSD1306_DrawPixel(uint8_t x, uint8_t y, bool on)
{
  uint16_t index;

  if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
  {
    return;
  }

  index = x + ((uint16_t)(y / 8U) * SSD1306_WIDTH);
  if (on)
  {
    ssd1306_buffer[index] |= (uint8_t)(1U << (y % 8U));
  }
  else
  {
    ssd1306_buffer[index] &= (uint8_t)~(1U << (y % 8U));
  }
}

static void SSD1306_DrawChar(uint8_t x, uint8_t y, char character, uint8_t scale)
{
  uint8_t glyph[5] = {0};
  uint8_t column;
  uint8_t row;
  uint8_t dx;
  uint8_t dy;

  if ((uint8_t)character < (sizeof(ssd1306_font) / sizeof(ssd1306_font[0])))
  {
    memcpy(glyph, ssd1306_font[(uint8_t)character], sizeof(glyph));
  }

  for (column = 0U; column < 5U; column++)
  {
    for (row = 0U; row < 8U; row++)
    {
      if ((glyph[column] & (1U << row)) != 0U)
      {
        for (dx = 0U; dx < scale; dx++)
        {
          for (dy = 0U; dy < scale; dy++)
          {
            SSD1306_DrawPixel((uint8_t)(x + (column * scale) + dx),
                              (uint8_t)(y + (row * scale) + dy),
                              true);
          }
        }
      }
    }
  }
}

static HAL_StatusTypeDef SSD1306_TryInitAtAddress(uint8_t address)
{
  static const uint8_t init_sequence[] = {
    0xAEU,
    0xD5U, 0x80U,
    0xA8U, 0x3FU,
    0xD3U, 0x00U,
    0x40U,
    0x8DU, 0x14U,
    0x20U, 0x00U,
    0xA1U,
    0xC8U,
    0xDAU, 0x12U,
    0x81U, 0xCFU,
    0xD9U, 0xF1U,
    0xDBU, 0x40U,
    0xA4U,
    0xA6U,
    0x21U, 0x00U, 0x7FU,
    0x22U, 0x00U, 0x07U,
    0xAFU
  };
  uint32_t index;

  ssd1306_address = address;
  for (index = 0U; index < sizeof(init_sequence); index++)
  {
    if (SSD1306_WriteCommand(init_sequence[index]) != HAL_OK)
    {
      return HAL_ERROR;
    }
  }

  return HAL_OK;
}

bool SSD1306_Init(SSD1306_WriteFunction write_function)
{
  ssd1306_write = write_function;
  ssd1306_ready = false;

  if (ssd1306_write == NULL)
  {
    return false;
  }

  if (SSD1306_TryInitAtAddress(SSD1306_PRIMARY_ADDRESS) != HAL_OK &&
      SSD1306_TryInitAtAddress(SSD1306_SECONDARY_ADDRESS) != HAL_OK)
  {
    return false;
  }

  ssd1306_ready = true;
  SSD1306_Clear();
  SSD1306_UpdateScreen();
  return true;
}

bool SSD1306_IsReady(void)
{
  return ssd1306_ready;
}

void SSD1306_Clear(void)
{
  if (!ssd1306_ready)
  {
    return;
  }

  SSD1306_ClearBuffer();
}

bool SSD1306_UpdateScreen(void)
{
  uint8_t page;

  if (!ssd1306_ready)
  {
    return false;
  }

  for (page = 0U; page < SSD1306_PAGES; page++)
  {
    if (SSD1306_WriteCommand((uint8_t)(0xB0U + page)) != HAL_OK ||
        SSD1306_WriteCommand(0x00U) != HAL_OK ||
        SSD1306_WriteCommand(0x10U) != HAL_OK ||
        SSD1306_WriteData(&ssd1306_buffer[page * SSD1306_WIDTH], SSD1306_WIDTH) != HAL_OK)
    {
      return false;
    }
  }

  return true;
}

void SSD1306_SetCursor(uint8_t x, uint8_t y)
{
  ssd1306_cursor_x = x;
  ssd1306_cursor_y = y;
}

void SSD1306_WriteString(const char *text)
{
  while ((text != NULL) && (*text != '\0') && ssd1306_ready)
  {
    SSD1306_DrawChar(ssd1306_cursor_x, ssd1306_cursor_y, *text, SSD1306_TEXT_SCALE);
    ssd1306_cursor_x = (uint8_t)(ssd1306_cursor_x + (6U * SSD1306_TEXT_SCALE));

    if (ssd1306_cursor_x > (SSD1306_WIDTH - (6U * SSD1306_TEXT_SCALE)))
    {
      return;
    }

    text++;
  }
}