/************************************************************
 * Analog Photo Sensor – Linear Distance Output
 *
 * MCU : ATtiny817
 * CPU : 10 MHz
 *
 * Features:
 *  - IR LED lock-in @ 5 kHz (TCA)
 *  - Differential ADC sampling
 *  - Envelope extraction + EMA
 *  - Envelope ? Distance LUT
 *  - Linear PWM output via TCB
 *  - UART debug
 ************************************************************/

#ifndef F_CPU
#define F_CPU 10000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

/* ===== Drivers ===== */
#include "../Drivers/TIMERS/TCA_pwm.h"
#include "../Drivers/TIMERS/TCB_pwm.h"
#include "../Drivers/ADC/adc.h"
#include "../Drivers/USART/usart.h"

/* =========================================================
 * Configuration
 * ========================================================= */

/* ---------- Lock-in ---------- */
#define IR_PWM_FREQ_HZ     5000
#define IR_PWM_DUTY_ON     50
#define HALF_PERIOD_US     100
#define IR_SETTLE_US       35
#define IR_SAMPLE_US       10

/* ---------- ADC ---------- */
#define ADC_CHANNEL        ADC_MUXPOS_AIN1_gc
#define ADC_AVG_SAMPLES    1

/* ---------- Envelope ---------- */
#define LOCKIN_ENVELOPE_SAMPLES  8

/* ---------- EMA ---------- */
#define EMA_SHIFT          3   /* N = 8 */

/* ---------- Output PWM ---------- */
#define OUTPUT_PWM_FREQ_HZ 60   /* Slow analog-style PWM */

/* ---------- Distance range ---------- */
#define DIST_MIN_MM        20
#define DIST_MAX_MM        300

/* =========================================================
 * Calibration LUT (REPLACE WITH REAL DATA)
 *
 * env_lut[] must be MONOTONIC DECREASING
 * dist_lut[] must be MONOTONIC INCREASING
 * ========================================================= */
static const uint16_t env_lut[] = {
  700,520,360,190,130,82,50,40,30,25,20  
};

static const uint16_t dist_lut_mm[] = {
     20,30,60, 90, 120,150,180,210,240,270,300
};

#define LUT_SIZE (sizeof(env_lut) / sizeof(env_lut[0]))

/* =========================================================
 * Envelope ? Distance (mm)
 * ========================================================= */
static uint16_t env_to_distance_mm(uint16_t env)
{
    if (env >= env_lut[0])
        return dist_lut_mm[0];

    for (uint8_t i = 1; i < LUT_SIZE; i++)
    {
        if (env > env_lut[i])
        {
            uint16_t x0 = env_lut[i - 1];
            uint16_t x1 = env_lut[i];
            uint16_t y0 = dist_lut_mm[i - 1];
            uint16_t y1 = dist_lut_mm[i];

            return y0 +
                (uint32_t)(x0 - env) * (y1 - y0) / (x0 - x1);
        }
    }

    return dist_lut_mm[LUT_SIZE - 1];
}

/* =========================================================
 * Distance ? PWM duty (LINEAR)
 * ========================================================= */
static uint8_t distance_to_pwm(uint16_t dist_mm)
{
    if (dist_mm <= DIST_MIN_MM)
        return 0;

    if (dist_mm >= DIST_MAX_MM)
        return 100;

    return (uint32_t)(dist_mm - DIST_MIN_MM) * 100 /
           (DIST_MAX_MM - DIST_MIN_MM);
}

/* =========================================================
 * MAIN
 * ========================================================= */
int main(void)
{
    /* ---------- ADC pin: PA1 / AIN1 ---------- */
    PORTA.DIRCLR = PIN1_bm;
    PORTA.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc;

    adc_init(ADC_REF_VDD);

    /* ---------- IR modulation (TCA) ---------- */
    tca_pwm_init(IR_PWM_FREQ_HZ, IR_PWM_DUTY_ON, TCA_PWM_WO0);
    tca_pwm_start();

    /* ---------- Output PWM (TCB ? PB5) ---------- */
    tcb_pwm_init(OUTPUT_PWM_FREQ_HZ, &PORTB, PIN5_bm);
    tcb_pwm_start();

    /* ---------- UART ---------- */
    uart_init(115200);
    uart_tx_string("Analog Photo Sensor – Linear Distance\r\n");

    sei();

    /* ---------- EMA state ---------- */
    uint32_t ema_value = 0;
    uint8_t ema_initialized = 0;

    while (1)
    {
        int32_t env_acc = 0;

        /* ===== Lock-in envelope ===== */
        for (uint8_t i = 0; i < LOCKIN_ENVELOPE_SAMPLES; i++)
        {
            /* IR ON */
            tca_pwm_set_duty(IR_PWM_DUTY_ON);
            _delay_us(IR_SETTLE_US);

            uint16_t adc_on =
                adc_read_avg(ADC_CHANNEL, ADC_AVG_SAMPLES);

            _delay_us(HALF_PERIOD_US - IR_SETTLE_US - IR_SAMPLE_US);

            /* IR OFF */
            tca_pwm_set_duty(0);
            _delay_us(IR_SETTLE_US);

            uint16_t adc_off =
                adc_read_avg(ADC_CHANNEL, ADC_AVG_SAMPLES);

            _delay_us(HALF_PERIOD_US - IR_SETTLE_US - IR_SAMPLE_US);

            int16_t diff = (int16_t)adc_on - (int16_t)adc_off;
            if (diff < 0)
                diff = -diff;

            env_acc += diff;
        }

        uint16_t envelope =
            env_acc / LOCKIN_ENVELOPE_SAMPLES;

        /* ===== EMA ===== */
        if (!ema_initialized)
        {
            ema_value = envelope << EMA_SHIFT;
            ema_initialized = 1;
        }
        else
        {
            ema_value +=
                ((uint32_t)envelope - (ema_value >> EMA_SHIFT));
        }

        uint16_t filtered_env =
            (uint16_t)(ema_value >> EMA_SHIFT);

        /* ===== Distance + PWM ===== */
        uint16_t distance_mm =
            env_to_distance_mm(filtered_env);

        uint8_t pwm_duty =
            distance_to_pwm(distance_mm);

        tcb_pwm_set_duty(pwm_duty);

        /* ===== Debug ===== */
        uart_tx_u16(filtered_env);
        uart_tx_char(',');
        uart_tx_u16(distance_mm);
        uart_tx_char(',');
        uart_tx_u16(pwm_duty);
        uart_tx_char('\n');
    }
}
