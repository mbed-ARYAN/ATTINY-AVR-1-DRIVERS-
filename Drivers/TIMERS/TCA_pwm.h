#ifndef TCA_PWM_H
#define TCA_PWM_H

#include <stdint.h>

/* TCA0 waveform outputs we support */
typedef enum {
    TCA_PWM_WO0 = 0,   // PB0
    TCA_PWM_WO1 = 1,   // PB1
    TCA_PWM_WO2 = 2    // PB2
} tca_pwm_channel_t;

/*
 * Initialize TCA0 PWM
 *
 * freq_hz      : PWM frequency in Hz
 * duty_percent : 0–100 (%)
 * channel      : WO0 / WO1 / WO2
 */
void tca_pwm_init(uint32_t freq_hz,
                  uint8_t duty_percent,
                  tca_pwm_channel_t channel);

void tca_pwm_start(void);
void tca_pwm_stop(void);

void tca_pwm_set_duty(uint8_t duty_percent);
uint16_t tca_pwm_get_counter(void);

#endif
