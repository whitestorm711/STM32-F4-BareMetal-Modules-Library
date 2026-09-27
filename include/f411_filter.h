
#ifndef F411_FILTER_H
#define F411_FILTER_H

#include "f411_platform.h"

int32_t F411_ClampI32(int32_t value, int32_t minimum, int32_t maximum);
int32_t F411_EMA_Q15(int32_t previous, int32_t input, uint16_t alpha_q15);
void F411_MovingAverageI32_Init(int32_t *buffer, uint16_t length, int64_t *sum);
int32_t F411_MovingAverageI32_Push(int32_t *buffer, uint16_t length, uint16_t *index, int64_t *sum, int32_t sample);

#endif
