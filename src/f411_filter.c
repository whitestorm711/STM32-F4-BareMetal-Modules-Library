
#include "f411_filter.h"

int32_t F411_ClampI32(int32_t value, int32_t minimum, int32_t maximum)
{
    if (value < minimum) return minimum;
    if (value > maximum) return maximum;
    return value;
}

int32_t F411_EMA_Q15(int32_t previous, int32_t input, uint16_t alpha_q15)
{
    int64_t delta;
    if (alpha_q15 > 32768U) alpha_q15 = 32768U;
    delta = (int64_t)input - previous;
    return (int32_t)(previous + ((delta * alpha_q15) >> 15));
}

void F411_MovingAverageI32_Init(int32_t *buffer, uint16_t length, int64_t *sum)
{
    if (buffer == NULL || sum == NULL || length == 0U) return;
    for (uint16_t i = 0U; i < length; i++) buffer[i] = 0;
    *sum = 0;
}

int32_t F411_MovingAverageI32_Push(int32_t *buffer, uint16_t length, uint16_t *index, int64_t *sum, int32_t sample)
{
    if (buffer == NULL || index == NULL || sum == NULL || length == 0U) return 0;
    if (*index >= length) *index = 0U;
    *sum -= buffer[*index];
    buffer[*index] = sample;
    *sum += sample;
    *index = (uint16_t)(*index + 1U);
    if (*index >= length) *index = 0U;
    return (int32_t)(*sum / length);
}
