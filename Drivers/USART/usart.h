#ifndef USART_H
#define USART_H

#include <stdint.h>
void uart_tx_i16(int16_t value);
void uart_init(uint32_t baud);
void uart_tx_char(char c);
void uart_tx_string(const char *s);
void uart_tx_u16(uint16_t value);

#endif
