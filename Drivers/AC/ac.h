#ifndef AC_H
#define AC_H

#include <stdint.h>

typedef enum {
	AC_POS_AINP0 = 0,
	AC_POS_AINP1 = 1,
	AC_POS_AINP2 = 2
} ac_pos_input_t;

typedef enum {
	AC_NEG_AINN   = 0,
	AC_NEG_DAC    = 1,
	AC_NEG_INTREF = 2
} ac_neg_input_t;

void ac_init(ac_pos_input_t pos, ac_neg_input_t neg);
void ac_enable(void);
void ac_disable(void);

void ac_enable_interrupt(void);
void ac_disable_interrupt(void);

uint8_t ac_get_output(void);

void ac_event(void);

#endif
