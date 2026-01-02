#include "usart.h"
#include <avr/io.h>
#include <stdlib.h>


/* Assuming F_CPU ? 3.33 MHz */
#define F_CPU_HZ 3333333UL

void uart_init(uint32_t baud)
{
	/* Calculate baud setting (datasheet formula) */
	uint16_t baud_reg = (uint16_t)((F_CPU_HZ * 64) / (16 * baud));

	USART0.BAUD = baud_reg;

	/* TX pin output */
	PORTB.DIRSET = PIN2_bm;   // PB2 = TXD (Xplained Mini)

	/* Enable transmitter */
	USART0.CTRLB = USART_TXEN_bm;
}

void uart_tx_char(char c)
{
	while (!(USART0.STATUS & USART_DREIF_bm))
	;
	USART0.TXDATAL = c;
}

void uart_tx_string(const char *s)
{
	while (*s)
	{
		uart_tx_char(*s++);
	}
}

void uart_tx_u16(uint16_t value)
{
	char buf[6];
	uint8_t i = 0;

	if (value == 0)
	{
		uart_tx_char('0');
		return;
	}

	while (value > 0)
	{
		buf[i++] = '0' + (value % 10);
		value /= 10;
	}

	while (i--)
	{
		uart_tx_char(buf[i]);
	}
	void uart_tx_i16(int16_t value)
	{
		if (value < 0)
		{
			uart_tx_char('-');
			value = -value;
		}

		uart_tx_u16((uint16_t)value);
	}


}
