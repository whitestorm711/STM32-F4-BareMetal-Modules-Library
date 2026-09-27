
#ifndef F411_CRC_H
#define F411_CRC_H

#include "f411_platform.h"

void F411_CRC_Reset(void);
uint32_t F411_CRC_CalcWords(const uint32_t *words, size_t count);
uint32_t F411_CRC_CalcOne(uint32_t word);

#endif
