/*
 * sw_timer.h
 *
 * Created: 19-12-2025 23:53:05
 *  Author: HP
 */ 


#ifndef SW_TIMER_H_
#define SW_TIMER_H_

#ifndef SW_TIMER_H
#define SW_TIMER_H

#include <stdint.h>

/* Software timer object */
typedef struct {
	volatile uint16_t remaining;
	volatile uint8_t  active;
} sw_timer_t;

/* Initialize software timer system */
void sw_timer_init(void);

/* Called from TCB ISR once per tick */
void sw_timer_tick(void);

/* Timer control API */
void sw_timer_start(sw_timer_t *t, uint16_t duration);
void sw_timer_stop(sw_timer_t *t);
void sw_timer_restart(sw_timer_t *t, uint16_t duration);
uint8_t sw_timer_expired(sw_timer_t *t);

/* Get system time in ticks */
uint32_t sw_timer_now(void);

#endif /* SW_TIMER_H */






#endif /* SW_TIMER_H_ */