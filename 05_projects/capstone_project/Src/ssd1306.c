#include <stdint.h>

#include "i2c.h"
#include "ssd1306.h"

#define SSD1306_PRIMARY_ADDR         0x3CU
#define SSD1306_SECONDARY_ADDR       0x3DU
#define SSD1306_WIDTH                128U
#define SSD1306_HEIGHT               64U
#define SSD1306_PAGES                8U
#define SSD1306_BUFFER_SIZE          1024U
#define GRAPH_WIDTH                  64U
#define GRAPH_HEIGHT                 24U
#define GRAPH_X                      64U
#define GRAPH_Y                      40U

static uint8_t oled_ready;
static uint8_t ssd1306_address = SSD1306_PRIMARY_ADDR;
static uint8_t ssd1306_buffer[SSD1306_BUFFER_SIZE];

static const uint8_t font_space[5] = {0x00U, 0x00U, 0x00U, 0x00U, 0x00U};
static const uint8_t font_colon[5] = {0x00U, 0x36U, 0x36U, 0x00U, 0x00U};
static const uint8_t font_digits[10][5] = {
  {0x3EU, 0x51U, 0x49U, 0x45U, 0x3EU},
  {0x00U, 0x42U, 0x7FU, 0x40U, 0x00U},
  {0x42U, 0x61U, 0x51U, 0x49U, 0x46U},
  {0x21U, 0x41U, 0x45U, 0x4BU, 0x31U},
  {0x18U, 0x14U, 0x12U, 0x7FU, 0x10U},
  {0x27U, 0x45U, 0x45U, 0x45U, 0x39U},
  {0x3CU, 0x4AU, 0x49U, 0x49U, 0x30U},
  {0x01U, 0x71U, 0x09U, 0x05U, 0x03U},
  {0x36U, 0x49U, 0x49U, 0x49U, 0x36U},
  {0x06U, 0x49U, 0x49U, 0x29U, 0x1EU}
};
static const uint8_t font_A[5] = {0x7EU, 0x11U, 0x11U, 0x11U, 0x7EU};
static const uint8_t font_B[5] = {0x7FU, 0x49U, 0x49U, 0x49U, 0x36U};
static const uint8_t font_C[5] = {0x7EU, 0x81U, 0x81U, 0x81U, 0x42U};
static const uint8_t font_D[5] = {0x7FU, 0x41U, 0x41U, 0x22U, 0x1CU};
static const uint8_t font_E[5] = {0x7FU, 0x49U, 0x49U, 0x49U, 0x41U};
static const uint8_t font_G[5] = {0x3EU, 0x41U, 0x49U, 0x49U, 0x7AU};
static const uint8_t font_I[5] = {0x00U, 0x41U, 0x7FU, 0x41U, 0x00U};
static const uint8_t font_L[5] = {0x7FU, 0x40U, 0x40U, 0x40U, 0x40U};
static const uint8_t font_M[5] = {0x7FU, 0x02U, 0x0CU, 0x02U, 0x7FU};
static const uint8_t font_N[5] = {0x7FU, 0x04U, 0x08U, 0x10U, 0x7FU};
static const uint8_t font_O[5] = {0x3EU, 0x41U, 0x41U, 0x41U, 0x3EU};
static const uint8_t font_P[5] = {0x7FU, 0x09U, 0x09U, 0x09U, 0x06U};
static const uint8_t font_R[5] = {0x7FU, 0x09U, 0x19U, 0x29U, 0x46U};
static const uint8_t font_S[5] = {0x46U, 0x49U, 0x49U, 0x49U, 0x31U};
static const uint8_t font_T[5] = {0x01U, 0x01U, 0x7FU, 0x01U, 0x01U};
static const uint8_t font_U[5] = {0x3FU, 0x40U, 0x40U, 0x40U, 0x3FU};
static const uint8_t font_V[5] = {0x1FU, 0x20U, 0x40U, 0x20U, 0x1FU};
static const uint8_t font_W[5] = {0x3FU, 0x40U, 0x38U, 0x40U, 0x3FU};

static const uint8_t *ssd1306_get_glyph(char c)
{
  if ((c >= '0') && (c <= '9'))
  {
    return font_digits[(uint8_t)(c - '0')];
  }

  switch (c)
  {
    case ' ': return font_space;
    case ':': return font_colon;
    case 'A': return font_A;
    case 'B': return font_B;
    case 'C': return font_C;
    case 'D': return font_D;
    case 'E': return font_E;
    case 'G': return font_G;
    case 'I': return font_I;
    case 'L': return font_L;
    case 'M': return font_M;
    case 'N': return font_N;
    case 'O': return font_O;
    case 'P': return font_P;
    case 'R': return font_R;
    case 'S': return font_S;
    case 'T': return font_T;
    case 'U': return font_U;
    case 'V': return font_V;
    case 'W': return font_W;
    default: return font_space;
  }
}

static void uint32_to_string(uint32_t value, char *buffer)
{
  char reversed[10];
  uint32_t index = 0U;
  uint32_t out_index = 0U;

  if (value == 0U)
  {
    buffer[0] = '0';
    buffer[1] = '\0';
    return;
  }

  while ((value > 0U) && (index < (uint32_t)sizeof(reversed)))
  {
    reversed[index++] = (char)('0' + (value % 10U));
    value /= 10U;
  }

  while (index > 0U)
  {
    buffer[out_index++] = reversed[--index];
  }

  buffer[out_index] = '\0';
}

static UINT ssd1306_write_command(uint8_t command)
{
  return i2c1_write_bytes(ssd1306_address, 0x00U, &command, 1U);
}

static UINT ssd1306_write_data_block(const uint8_t *data, uint16_t length)
{
  return i2c1_write_bytes(ssd1306_address, 0x40U, data, length);
}

static void ssd1306_set_cursor(uint8_t page, uint8_t column)
{
  if (oled_ready == 0U)
  {
    return;
  }

  (void)ssd1306_write_command((uint8_t)(0xB0U | (page & 0x07U)));
  (void)ssd1306_write_command((uint8_t)(0x00U | (column & 0x0FU)));
  (void)ssd1306_write_command((uint8_t)(0x10U | ((column >> 4) & 0x0FU)));
}

static void ssd1306_clear_buffer(void)
{
  uint32_t index;

  for (index = 0U; index < sizeof(ssd1306_buffer); index++)
  {
    ssd1306_buffer[index] = 0U;
  }
}

static void ssd1306_draw_pixel(uint8_t x, uint8_t y)
{
  uint16_t index;

  if ((x >= SSD1306_WIDTH) || (y >= SSD1306_HEIGHT))
  {
    return;
  }

  index = (uint16_t)x + ((uint16_t)(y / 8U) * SSD1306_WIDTH);
  ssd1306_buffer[index] |= (uint8_t)(1U << (y % 8U));
}

static void ssd1306_draw_hline(uint8_t x, uint8_t y, uint8_t length)
{
  uint8_t index;

  for (index = 0U; index < length; index++)
  {
    ssd1306_draw_pixel((uint8_t)(x + index), y);
  }
}

static void ssd1306_draw_vline(uint8_t x, uint8_t y, uint8_t length)
{
  uint8_t index;

  for (index = 0U; index < length; index++)
  {
    ssd1306_draw_pixel(x, (uint8_t)(y + index));
  }
}

static void ssd1306_draw_alert_icon(uint8_t x, uint8_t y)
{
  ssd1306_draw_pixel((uint8_t)(x + 3U), y);
  ssd1306_draw_hline((uint8_t)(x + 2U), (uint8_t)(y + 1U), 3U);
  ssd1306_draw_hline((uint8_t)(x + 1U), (uint8_t)(y + 2U), 5U);
  ssd1306_draw_hline(x, (uint8_t)(y + 3U), 7U);
  ssd1306_draw_hline((uint8_t)(x + 1U), (uint8_t)(y + 4U), 5U);
  ssd1306_draw_hline((uint8_t)(x + 2U), (uint8_t)(y + 5U), 3U);
  ssd1306_draw_pixel((uint8_t)(x + 3U), (uint8_t)(y + 6U));
  ssd1306_draw_vline((uint8_t)(x + 3U), (uint8_t)(y + 2U), 3U);
  ssd1306_draw_pixel((uint8_t)(x + 3U), (uint8_t)(y + 6U));
}

static void ssd1306_draw_graph(const uint16_t *ring_buffer, uint8_t ring_size, uint8_t ring_head, uint8_t ring_count)
{
  uint8_t graph_index;
  uint8_t source_index;
  uint16_t sample;
  uint8_t y;

  ssd1306_draw_hline(GRAPH_X, (uint8_t)(GRAPH_Y + GRAPH_HEIGHT), GRAPH_WIDTH);
  ssd1306_draw_vline(GRAPH_X, (uint8_t)(GRAPH_Y + 1U), GRAPH_HEIGHT);

  if ((ring_buffer == 0) || (ring_size == 0U))
  {
    return;
  }

  for (graph_index = 0U; graph_index < GRAPH_WIDTH; graph_index++)
  {
    if (graph_index >= ring_count)
    {
      continue;
    }

    source_index = (uint8_t)((ring_head + ring_size - ring_count + graph_index) % ring_size);
    sample = ring_buffer[source_index];
    y = (uint8_t)(GRAPH_Y + GRAPH_HEIGHT - ((sample * GRAPH_HEIGHT) / 4095U));
    ssd1306_draw_pixel((uint8_t)(GRAPH_X + graph_index), y);
  }
}

static void ssd1306_draw_char(uint8_t x, uint8_t y, char c)
{
  const uint8_t *glyph;
  uint8_t column;
  uint8_t row;

  glyph = ssd1306_get_glyph(c);

  for (column = 0U; column < 5U; column++)
  {
    for (row = 0U; row < 8U; row++)
    {
      if ((glyph[column] & (1U << row)) != 0U)
      {
        ssd1306_draw_pixel((uint8_t)(x + column), (uint8_t)(y + row));
      }
    }
  }
}

static UINT ssd1306_update_screen(void)
{
  uint8_t page;

  if (oled_ready == 0U)
  {
    return TX_NOT_AVAILABLE;
  }

  for (page = 0U; page < SSD1306_PAGES; page++)
  {
    ssd1306_set_cursor(page, 0U);
    if (ssd1306_write_data_block(&ssd1306_buffer[page * SSD1306_WIDTH], SSD1306_WIDTH) != TX_SUCCESS)
    {
      return TX_NOT_AVAILABLE;
    }
  }

  return TX_SUCCESS;
}

static uint8_t ssd1306_string_width(const char *str)
{
  uint8_t width = 0U;

  while (*str != '\0')
  {
    width = (uint8_t)(width + 6U);
    str++;
  }

  return width;
}

static void ssd1306_write_string_centered(uint8_t page, const char *str)
{
  uint8_t text_width = ssd1306_string_width(str);
  uint8_t start_column = (uint8_t)((SSD1306_WIDTH - text_width) / 2U);
  uint8_t cursor_x = start_column;

  while (*str != '\0')
  {
    ssd1306_draw_char(cursor_x, (uint8_t)(page * 8U), *str++);
    cursor_x = (uint8_t)(cursor_x + 6U);
  }
}

static void ssd1306_write_string(uint8_t x, uint8_t page, const char *str)
{
  uint8_t cursor_x = x;

  while ((*str != '\0') && (cursor_x <= 122U))
  {
    ssd1306_draw_char(cursor_x, (uint8_t)(page * 8U), *str++);
    cursor_x = (uint8_t)(cursor_x + 6U);
  }
}

static void ssd1306_render_status(uint16_t value,
                                  uint16_t average,
                                  uint8_t button,
                                  uint8_t alert_active,
                                  const uint16_t *ring_buffer,
                                  uint8_t ring_size,
                                  uint8_t ring_head,
                                  uint8_t ring_count)
{
  char adc_text[16] = "ADC:";
  char avg_text[16] = "AVG:";
  char button_text[16] = "BTN:";

  if (oled_ready == 0U)
  {
    return;
  }

  uint32_to_string(value, &adc_text[4]);
  uint32_to_string(average, &avg_text[4]);
  button_text[4] = (button != 0U) ? '1' : '0';
  button_text[5] = '\0';

  ssd1306_clear_buffer();
  ssd1306_write_string_centered(0U, "WELCOME");
  ssd1306_write_string(0U, 2U, adc_text);
  ssd1306_write_string(0U, 3U, avg_text);
  ssd1306_write_string(0U, 4U, button_text);
  ssd1306_draw_graph(ring_buffer, ring_size, ring_head, ring_count);
  if (alert_active != 0U)
  {
    ssd1306_draw_alert_icon(118U, 0U);
  }
}

static UINT ssd1306_try_init_at_address(uint8_t address)
{
  static const uint8_t init_sequence[] = {
    0xAEU, 0xD5U, 0x80U, 0xA8U, 0x3FU, 0xD3U, 0x00U, 0x40U,
    0x8DU, 0x14U, 0x20U, 0x00U, 0xA1U, 0xC8U, 0xDAU, 0x12U,
    0x81U, 0xCFU, 0xD9U, 0xF1U, 0xDBU, 0x40U, 0xA4U, 0xA6U,
    0xAFU
  };

  ssd1306_address = address;
  if (i2c1_write_bytes(ssd1306_address, 0x00U, init_sequence, (uint16_t)sizeof(init_sequence)) != TX_SUCCESS)
  {
    return TX_NOT_AVAILABLE;
  }

  return TX_SUCCESS;
}

void ssd1306_init(void)
{
  oled_ready = 0U;

  if ((ssd1306_try_init_at_address(SSD1306_PRIMARY_ADDR) == TX_SUCCESS) ||
      (ssd1306_try_init_at_address(SSD1306_SECONDARY_ADDR) == TX_SUCCESS))
  {
    oled_ready = 1U;
    ssd1306_clear_buffer();
  }
}

uint8_t ssd1306_is_ready(void)
{
  return oled_ready;
}

UINT ssd1306_render_dashboard(uint16_t value,
                              uint16_t average,
                              uint8_t button,
                              uint8_t alert_active,
                              const uint16_t *ring_buffer,
                              uint8_t ring_size,
                              uint8_t ring_head,
                              uint8_t ring_count)
{
  if (oled_ready == 0U)
  {
    return TX_NOT_AVAILABLE;
  }

  ssd1306_render_status(value, average, button, alert_active, ring_buffer, ring_size, ring_head, ring_count);

  if (ssd1306_update_screen() != TX_SUCCESS)
  {
    ssd1306_init();
    if (oled_ready == 0U)
    {
      return TX_NOT_AVAILABLE;
    }

    ssd1306_render_status(value, average, button, alert_active, ring_buffer, ring_size, ring_head, ring_count);
    return ssd1306_update_screen();
  }

  return TX_SUCCESS;
}