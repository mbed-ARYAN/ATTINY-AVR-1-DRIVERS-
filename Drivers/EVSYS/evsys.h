#ifndef EVSYS_H
#define EVSYS_H

#include <stdint.h>

typedef enum {
	EVSYS_CH0 = 0,
	EVSYS_CH1,
	EVSYS_CH2,
	EVSYS_CH3,
	EVSYS_CH4,
	EVSYS_CH5
} evsys_channel_t;

/* API */
void evsys_reset(void);
void evsys_channel_set_generator(evsys_channel_t ch, uint8_t generator);
void evsys_user_connect(uint8_t user_index, evsys_channel_t ch);
void evsys_user_disconnect(uint8_t user_index);

#endif
