/*
 * sw_timer.c
 *
 * Created: 19-12-2025 23:52:48
 *  Author: HP
 */ 
#include "sw_timer.h"

/* Number of software timers in the system */
#define MAX_SW_TIMERS 8

/* Internal timer storage */
static sw_timer_t timers[MAX_SW_TIMERS];

/* Monotonic system tick counter */
static volatile uint32_t system_ticks = 0;

void sw_timer_init(void)
{
	for (uint8_t i = 0; i < MAX_SW_TIMERS; i++)
	{
		timers[i].active = 0;
		timers[i].remaining = 0;
	}

	system_ticks = 0;
}

void sw_timer_tick(void)
{
	system_ticks++;

	for (uint8_t i = 0; i < MAX_SW_TIMERS; i++)
	{
		if (timers[i].active && timers[i].remaining > 0)
		{
			timers[i].remaining--;

			if (timers[i].remaining == 0)
			{
				timers[i].active = 0;
			}
		}
	}
}

void sw_timer_start(sw_timer_t *t, uint16_t duration)
{
	t->remaining = duration;
	t->active = 1;
}

void sw_timer_stop(sw_timer_t *t)
{
	t->active = 0;
}

void sw_timer_restart(sw_timer_t *t, uint16_t duration)
{
	t->remaining = duration;
	t->active = 1;
}

uint8_t sw_timer_expired(sw_timer_t *t)
{
	return (t->active == 0 && t->remaining == 0);
}

uint32_t sw_timer_now(void)
{
	return system_ticks;
}
