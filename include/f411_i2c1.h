
#ifndef F411_I2C1_H
#define F411_I2C1_H

#include "f411_platform.h"

int F411_I2C1_Init(uint32_t pclk1_hz, uint32_t scl_hz);
int F411_I2C1_Write7(uint8_t address7, const uint8_t *data, size_t length, uint32_t timeout_loops);
int F411_I2C1_Read7(uint8_t address7, uint8_t *data, size_t length, uint32_t timeout_loops);
int F411_I2C1_MemWrite8(uint8_t address7, uint8_t reg, const uint8_t *data, size_t length, uint32_t timeout_loops);
int F411_I2C1_MemRead8(uint8_t address7, uint8_t reg, uint8_t *data, size_t length, uint32_t timeout_loops);

#endif
