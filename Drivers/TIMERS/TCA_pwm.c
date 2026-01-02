#include "TCA_pwm.h"
#include <avr/io.h>

/* ================= CONFIGURATION ================= */

/* Effective CPU clock (documented choice) */
#define F_CPU_HZ 3333333UL

/* ================= INTERNAL STATE ================= */

static uint16_t pwm_period;
static tca_pwm_channel_t active_channel;

/* --------------------------------------------------
 * Configure output pin (PB0 / PB1 / PB2)
 * -------------------------------------------------- */
static void tca_pwm_configure_pin(tca_pwm_channel_t ch)
{
    switch (ch)
    {
        case TCA_PWM_WO0:   // PB0
            PORTB.DIRSET = (1 << 0);
            PORTB.OUTCLR = (1 << 0);
            PORTB.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc;
            break;

        case TCA_PWM_WO1:   // PB1
            PORTB.DIRSET = (1 << 1);
            PORTB.OUTCLR = (1 << 1);
            PORTB.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc;
            break;

        case TCA_PWM_WO2:   // PB2
            PORTB.DIRSET = (1 << 2);
            PORTB.OUTCLR = (1 << 2);
            PORTB.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
            break;
    }
}

/* --------------------------------------------------
 * Initialize PWM (frequency + duty based)
 * -------------------------------------------------- */
void tca_pwm_init(uint32_t freq_hz,
                  uint8_t duty_percent,
                  tca_pwm_channel_t channel)
{
    /* Clamp inputs */
    if (freq_hz == 0) freq_hz = 1;
    if (duty_percent > 100) duty_percent = 100;

    /* Disable TCA */
    TCA0.SINGLE.CTRLA = 0;

    /* Compute period */
    pwm_period = (uint16_t)(F_CPU_HZ / freq_hz);

    if (pwm_period == 0)
        pwm_period = 1;

    /* Compute compare */
    uint16_t cmp =
        (uint32_t)pwm_period * duty_percent / 100;

    active_channel = channel;

    /* Configure output pin */
    tca_pwm_configure_pin(channel);

    /* Program timer */
    TCA0.SINGLE.PER = pwm_period;

    /* Clear all compare registers */
    TCA0.SINGLE.CMP0 = 0;
    TCA0.SINGLE.CMP1 = 0;
    TCA0.SINGLE.CMP2 = 0;

    /* PWM mode */
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc;

    /* Enable selected channel */
    switch (channel)
    {
        case TCA_PWM_WO0:
            TCA0.SINGLE.CMP0 = cmp;
            TCA0.SINGLE.CTRLB |= TCA_SINGLE_CMP0EN_bm;
            break;

        case TCA_PWM_WO1:
            TCA0.SINGLE.CMP1 = cmp;
            TCA0.SINGLE.CTRLB |= TCA_SINGLE_CMP1EN_bm;
            break;

        case TCA_PWM_WO2:
            TCA0.SINGLE.CMP2 = cmp;
            TCA0.SINGLE.CTRLB |= TCA_SINGLE_CMP2EN_bm;
            break;
    }

    /* Known phase start */
    TCA0.SINGLE.CNT = 0;

    /* Clear flags */
    TCA0.SINGLE.INTFLAGS =
        TCA_SINGLE_OVF_bm |
        TCA_SINGLE_CMP0_bm |
        TCA_SINGLE_CMP1_bm |
        TCA_SINGLE_CMP2_bm;
}

/* --------------------------------------------------
 * Start / Stop
 * -------------------------------------------------- */
void tca_pwm_start(void)
{
    TCA0.SINGLE.CTRLA =
        TCA_SINGLE_ENABLE_bm |
        TCA_SINGLE_CLKSEL_DIV1_gc;
}

void tca_pwm_stop(void)
{
    TCA0.SINGLE.CTRLA = 0;
}

/* --------------------------------------------------
 * Update duty cycle (%)
 * -------------------------------------------------- */
void tca_pwm_set_duty(uint8_t duty_percent)
{
    if (duty_percent > 100)
        duty_percent = 100;

    uint16_t cmp =
        (uint32_t)pwm_period * duty_percent / 100;

    switch (active_channel)
    {
        case TCA_PWM_WO0: TCA0.SINGLE.CMP0 = cmp; break;
        case TCA_PWM_WO1: TCA0.SINGLE.CMP1 = cmp; break;
        case TCA_PWM_WO2: TCA0.SINGLE.CMP2 = cmp; break;
    }
}

/* --------------------------------------------------
 * Debug helper
 * -------------------------------------------------- */
uint16_t tca_pwm_get_counter(void)
{
    return TCA0.SINGLE.CNT;
}
