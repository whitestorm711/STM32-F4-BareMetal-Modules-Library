
#include "f411_reset.h"

uint32_t F411_ResetFlags(void)
{
    return RCC->CSR;
}

uint8_t F411_ResetWas(uint8_t flag_bit)
{
    if (flag_bit > 31U) return 0U;
    return (uint8_t)((RCC->CSR >> flag_bit) & 1U);
}

void F411_ResetFlagsClear(void)
{
    RCC->CSR |= (1U << 24); /* RMVF */
}

void F411_SoftwareReset(void)
{
    volatile uint32_t *aircr = (volatile uint32_t *)0xE000ED0CU;
    uint32_t reg = *aircr;
    reg &= ~(0xFFFFU << 16);
    reg |= (0x5FAU << 16);
    reg |= (1U << 2); /* SYSRESETREQ */
    *aircr = reg;
    while (1) {}
}
