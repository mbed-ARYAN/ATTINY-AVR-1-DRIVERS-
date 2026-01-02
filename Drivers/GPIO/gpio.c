/*
 * gpio.c
 *
 * Created: 28-12-2025 22:48:08
 *  Author: HP
 */ 
#include "gpio.h"

/* ---------- Direction ---------- */

void gpio_set_direction(PORT_t *port, uint8_t pin, gpio_dir_t dir)
{
	if (dir == GPIO_OUTPUT)
	{
		port->DIRSET = (1 << pin);
	}
	else
	{
		port->DIRCLR = (1 << pin);
	}
}

/* ---------- Output ---------- */

void gpio_write(PORT_t *port, uint8_t pin, uint8_t value)
{
	if (value)
	{
		port->OUTSET = (1 << pin);
	}
	else
	{
		port->OUTCLR = (1 << pin);
	}
}

void gpio_toggle(PORT_t *port, uint8_t pin)
{
	port->OUTTGL = (1 << pin);
}

/* ---------- Input ---------- */

uint8_t gpio_read(PORT_t *port, uint8_t pin)
{
	return (port->IN & (1 << pin)) ? 1 : 0;
}

/* ---------- Pin configuration ---------- */

void gpio_configure_pin(
PORT_t *port,
uint8_t pin,
gpio_pull_t pull,
gpio_inbuf_t inbuf
)
{
	uint8_t cfg = 0;

	if (pull == GPIO_PULLUP)
	{
		cfg |= PORT_PULLUPEN_bm;
	}

	if (inbuf == GPIO_INBUF_DISABLE)
	{
		cfg |= PORT_ISC_INPUT_DISABLE_gc;
	}
	else
	{
		cfg |= PORT_ISC_INTDISABLE_gc;
	}

	/* tinyAVR-1: PINnCTRL registers are contiguous */
	volatile uint8_t *pinctrl = &port->PIN0CTRL;
	pinctrl[pin] = cfg;
}