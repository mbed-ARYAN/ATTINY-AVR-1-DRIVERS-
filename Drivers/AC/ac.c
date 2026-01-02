#include "ac.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void ac_init(ac_pos_input_t pos, ac_neg_input_t neg)
{
    /* Disable AC before configuration */
    AC0.CTRLA = 0;

    /* MUXCTRLA encoding:
       bits 3:0 ? MUXPOS
       bits 7:4 ? MUXNEG */
    AC0.MUXCTRLA = (neg << 4) | (pos & 0x0F);

    /* Enable hysteresis (only option: ~50 mV on ATtiny817) */
    AC0.CTRLA = AC_HYSMODE_50mV_gc;

    /* Clear comparator state */
    AC0.STATUS = AC_STATE_bm;
}

void ac_enable(void)
{
    AC0.CTRLA |= AC_ENABLE_bm;
}

void ac_disable(void)
{
    AC0.CTRLA &= ~AC_ENABLE_bm;
}

void ac_enable_interrupt(void)
{
    AC0.INTCTRL = AC_CMP_bm;
}

void ac_disable_interrupt(void)
{
    AC0.INTCTRL = 0;
}

uint8_t ac_get_output(void)
{
    return (AC0.STATUS & AC_STATE_bm) ? 1 : 0;
}

__attribute__((weak))
void ac_event(void)
{
}

ISR(AC0_AC_vect)
{
    AC0.STATUS = AC_STATE_bm;   /* Clear flag */
    ac_event();
}
