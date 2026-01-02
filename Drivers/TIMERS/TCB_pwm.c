#include "TCB_pwm.h"
#include <avr/interrupt.h>

/* =========================================================
 * CPU clock definition
 * ========================================================= */
#ifndef F_CPU
#define F_CPU 3333333UL     // 3.33 MHz system clock
#endif

/* =========================================================
 * Internal state
 * ========================================================= */
static volatile PORT_t *pwm_port;
static uint8_t pwm_pin;

static uint16_t period_ticks;
static uint16_t on_ticks;
static uint8_t  duty_percent;

static uint8_t phase;   // 0 = ON phase, 1 = OFF phase

/* =========================================================
 * Init
 * ========================================================= */
void tcb_pwm_init(uint16_t freq_hz,
                  volatile PORT_t *port,
                  uint8_t pin_bm)
{
    pwm_port = port;
    pwm_pin  = pin_bm;

    /* Configure output pin */
    pwm_port->DIRSET = pwm_pin;
    pwm_port->OUTCLR = pwm_pin;

    /* TCB clock = CLK_PER (DIV1) */
    period_ticks = (uint32_t)F_CPU / freq_hz;
    if (period_ticks < 2)
        period_ticks = 2;

    duty_percent = 0;
    on_ticks     = 0;
    phase        = 0;

    /* Stop TCB */
    TCB0.CTRLA = 0;

    /* Periodic interrupt mode */
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;

    /* Initial compare */
    TCB0.CCMP = period_ticks;

    /* Enable interrupt */
    TCB0.INTCTRL = TCB_CAPT_bm;
}

/* =========================================================
 * Control
 * ========================================================= */
void tcb_pwm_start(void)
{
    phase = 0;
    TCB0.CCMP  = period_ticks;
    TCB0.CTRLA = TCB_ENABLE_bm | TCB_CLKSEL_CLKDIV1_gc;
}

void tcb_pwm_stop(void)
{
    TCB0.CTRLA = 0;
    pwm_port->OUTCLR = pwm_pin;
}

void tcb_pwm_set_duty(uint8_t duty)
{
    if (duty > 100)
        duty = 100;

    duty_percent = duty;
    on_ticks = (uint32_t)period_ticks * duty / 100;
}

void tcb_pwm_set_duty_ticks(uint16_t ticks)
{
    if (ticks >= period_ticks)
        ticks = period_ticks;

    on_ticks = ticks;

    if (ticks == 0)
        duty_percent = 0;
    else if (ticks == period_ticks)
        duty_percent = 100;
    else
        duty_percent = (uint32_t)ticks * 100 / period_ticks;
}

uint16_t tcb_pwm_get_period_ticks(void)
{
    return period_ticks;
}

/* =========================================================
 * ISR — phase-based PWM engine
 * ========================================================= */
ISR(TCB0_INT_vect)
{
    TCB0.INTFLAGS = TCB_CAPT_bm;

    /* Special cases */
    if (duty_percent == 0)
    {
        pwm_port->OUTCLR = pwm_pin;
        TCB0.CCMP = period_ticks;
        phase = 0;
        return;
    }

    if (duty_percent >= 100)
    {
        pwm_port->OUTSET = pwm_pin;
        TCB0.CCMP = period_ticks;
        phase = 0;
        return;
    }

    /* Normal PWM */
    if (phase == 0)
    {
        /* ON phase */
        pwm_port->OUTSET = pwm_pin;
        TCB0.CCMP = on_ticks;
        phase = 1;
    }
    else
    {
        /* OFF phase */
        pwm_port->OUTCLR = pwm_pin;
        TCB0.CCMP = period_ticks - on_ticks;
        phase = 0;
    }
}
