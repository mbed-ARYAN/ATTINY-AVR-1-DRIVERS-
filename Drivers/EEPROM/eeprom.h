/*
 * eeprom.h
 *
 * Created: 28-12-2025 00:43:31
 *  Author: HP
 */ 

#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>

/* Single-byte access */
uint8_t ee_read_byte(uint16_t addr);
void    ee_write_byte(uint16_t addr, uint8_t data);

/* Block access */
void ee_read(uint16_t addr, void *buf, uint16_t len);
void ee_write(uint16_t addr, const void *buf, uint16_t len);

#endif
