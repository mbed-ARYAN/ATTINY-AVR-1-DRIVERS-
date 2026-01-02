#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <avr/io.h>

/* Direction */
typedef enum {
	GPIO_INPUT = 0,
	GPIO_OUTPUT
} gpio_dir_t;

/* Pull configuration */
typedef enum {
	GPIO_NOPULL = 0,
	GPIO_PULLUP
} gpio_pull_t;

/* Input buffer control */
typedef enum {
	GPIO_INBUF_ENABLE = 0,
	GPIO_INBUF_DISABLE
} gpio_inbuf_t;

/* API */
void gpio_set_direction(PORT_t *port, uint8_t pin, gpio_dir_t dir);
void gpio_write(PORT_t *port, uint8_t pin, uint8_t value);
uint8_t gpio_read(PORT_t *port, uint8_t pin);
void gpio_toggle(PORT_t *port, uint8_t pin);

void gpio_configure_pin(
PORT_t *port,
uint8_t pin,
gpio_pull_t pull,
gpio_inbuf_t inbuf
);

#endif /* GPIO_H */
