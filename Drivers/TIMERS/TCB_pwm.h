#ifndef TCB_PWM_H
#define TCB_PWM_H

#include <stdint.h>
#include <avr/io.h>

/*
 * Initialize TCB0-based PWM
 *
 * freq_hz : PWM frequency (e.g. 60)
 * port    : GPIO port (e.g. &PORTB)
 * pin_bm  : pin bit mask (e.g. PIN5_bm)
 */
void tcb_pwm_init(uint16_t freq_hz,
                  volatile PORT_t *port,
                  uint8_t pin_bm);

/* Start / Stop PWM */
void tcb_pwm_start(void);
void tcb_pwm_stop(void);

/* Coarse duty control: 0–100 (%) */
void tcb_pwm_set_duty(uint8_t duty_percent);

/* High-resolution duty control: 0–period_ticks */
void tcb_pwm_set_duty_ticks(uint16_t ticks);

/* Query resolution */
uint16_t tcb_pwm_get_period_ticks(void);

#endif
