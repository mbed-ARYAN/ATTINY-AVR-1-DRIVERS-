#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/* ADC reference selection */
typedef enum {
	ADC_REF_VDD,
	ADC_REF_INT_1V1,
	ADC_REF_INT_2V5
} adc_ref_t;

/* API */
void     adc_init(adc_ref_t ref);
void     adc_select_channel(uint8_t muxpos);
void     adc_start_conversion(void);
uint8_t  adc_is_ready(void);
uint16_t adc_read_result(void);

/* Convenience blocking read */
uint16_t adc_read_blocking(uint8_t muxpos);

/* Simple averaging helper */
uint16_t adc_read_avg(uint8_t muxpos, uint8_t samples);

#endif
