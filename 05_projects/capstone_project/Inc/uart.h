#ifndef UART_H
#define UART_H

#include <stdint.h>

#include "tx_api.h"

void usart1_init(void);
void usart1_send_char(char c);
void usart1_send_string(const char *str);
char usart1_receive_char(void);
UINT usart1_data_available(void);
void usart1_send_uint32(uint32_t value);

#endif