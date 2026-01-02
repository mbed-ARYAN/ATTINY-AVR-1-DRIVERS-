


#include "eeprom.h"
#include <avr/eeprom.h>

uint8_t ee_read_byte(uint16_t addr)
{
	return eeprom_read_byte((uint8_t *)addr);
}

void ee_write_byte(uint16_t addr, uint8_t data)
{
	eeprom_update_byte((uint8_t *)addr, data);
}

void ee_read(uint16_t addr, void *buf, uint16_t len)
{
	eeprom_read_block(buf, (const void *)addr, len);
}

void ee_write(uint16_t addr, const void *buf, uint16_t len)
{
	eeprom_update_block(buf, (void *)addr, len);
}
