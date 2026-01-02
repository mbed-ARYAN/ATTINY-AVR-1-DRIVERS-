#include "evsys.h"
#include <avr/io.h>

/* Offsets from EVSYS base (datasheet-defined) */
#define EVSYS_CHANNEL_OFFSET  0x00  /* CHANNEL0..5 */
#define EVSYS_USER_OFFSET     0x10  /* USER0..n   */

static inline volatile uint8_t *evsys_reg(uint8_t offset)
{
	return &((volatile uint8_t *)&EVSYS)[offset];
}

void evsys_reset(void)
{
	for (uint8_t i = 0; i < 6; i++)
	{
		*evsys_reg(EVSYS_CHANNEL_OFFSET + i) = 0;
	}
}

void evsys_channel_set_generator(evsys_channel_t ch, uint8_t generator)
{
	*evsys_reg(EVSYS_CHANNEL_OFFSET + ch) = generator;
}

void evsys_user_connect(uint8_t user_index, evsys_channel_t ch)
{
	/* USER registers expect (channel + 1), 0 = disconnected */
	*evsys_reg(EVSYS_USER_OFFSET + user_index) = (uint8_t)(ch + 1);
}

void evsys_user_disconnect(uint8_t user_index)
{
	*evsys_reg(EVSYS_USER_OFFSET + user_index) = 0;
}
