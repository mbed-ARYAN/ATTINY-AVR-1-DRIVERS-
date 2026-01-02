#include "adc.h"
#include <avr/io.h>

/* -------------------------------------------------
 * Initialize ADC
 * ------------------------------------------------- */
void adc_init(adc_ref_t ref)
{
    /* Disable ADC before config */
    ADC0.CTRLA = 0;

    /* Reference selection */
    switch (ref)
    {
        case ADC_REF_VDD:
            ADC0.CTRLC = ADC_REFSEL_VDDREF_gc;
            break;

        case ADC_REF_INT_1V1:
            ADC0.CTRLC = ADC_REFSEL_INTREF_gc;
            break;

        case ADC_REF_INT_2V5:
            ADC0.CTRLC = ADC_REFSEL_INTREF_gc;
            break;
    }

    /* ADC clock prescaler:
     * 3.33 MHz / 32 ? 104 kHz (datasheet-safe)
     */
    ADC0.CTRLC |= ADC_PRESC_DIV32_gc;

    /* Enable ADC */
    ADC0.CTRLA = ADC_ENABLE_bm;
}

/* -------------------------------------------------
 * Select ADC channel
 * ------------------------------------------------- */
void adc_select_channel(uint8_t muxpos)
{
    ADC0.MUXPOS = muxpos;
}

/* -------------------------------------------------
 * Start conversion
 * ------------------------------------------------- */
void adc_start_conversion(void)
{
    ADC0.COMMAND = ADC_STCONV_bm;
}

/* -------------------------------------------------
 * Check if result ready
 * ------------------------------------------------- */
uint8_t adc_is_ready(void)
{
    return (ADC0.INTFLAGS & ADC_RESRDY_bm);
}

/* -------------------------------------------------
 * Read ADC result
 * ------------------------------------------------- */
uint16_t adc_read_result(void)
{
    /* Clear flag */
    ADC0.INTFLAGS = ADC_RESRDY_bm;
    return ADC0.RES;
}

/* -------------------------------------------------
 * Blocking read (debug-friendly)
 * ------------------------------------------------- */
uint16_t adc_read_blocking(uint8_t muxpos)
{
    adc_select_channel(muxpos);
    adc_start_conversion();

    while (!adc_is_ready())
        ;

    return adc_read_result();
}

/* -------------------------------------------------
 * Averaged read (simple noise reduction)
 * ------------------------------------------------- */
uint16_t adc_read_avg(uint8_t muxpos, uint8_t samples)
{
    uint32_t sum = 0;

    for (uint8_t i = 0; i < samples; i++)
    {
        sum += adc_read_blocking(muxpos);
    }

    return (uint16_t)(sum / samples);
}
