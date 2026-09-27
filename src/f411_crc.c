
#include "f411_crc.h"
#include "f411_rcc.h"

void F411_CRC_Reset(void)
{
    F411_RCC_EnableCRC();
    CRC->CR = (1U << 0);
}

uint32_t F411_CRC_CalcWords(const uint32_t *words, size_t count)
{
    if (words == NULL && count != 0U) return 0U;
    F411_CRC_Reset();
    for (size_t i = 0U; i < count; i++) CRC->DR = words[i];
    return CRC->DR;
}

uint32_t F411_CRC_CalcOne(uint32_t word)
{
    return F411_CRC_CalcWords(&word, 1U);
}
