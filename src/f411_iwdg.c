
#include "f411_iwdg.h"
#include "f411_rcc.h"

int F411_IWDG_Init(uint8_t prescaler_code, uint16_t reload, uint32_t timeout_loops)
{
    if (prescaler_code > 7U || reload > 0x0FFFU) return -1;

    /* LSI is the IWDG clock source on STM32F411. */
    RCC->CSR |= (1U << 0); /* LSION */
    while ((RCC->CSR & (1U << 1)) == 0U)
    {
        if (timeout_loops == 0U) return -2;
        timeout_loops--;
    }

    IWDG->KR = 0x00005555U; /* unlock PR/RLR */

    IWDG->PR = (uint32_t)(prescaler_code & 7U);
    IWDG->RLR = (uint32_t)(reload & 0x0FFFU);

    while (IWDG->SR != 0U)
    {
        if (timeout_loops == 0U) return -3;
        timeout_loops--;
    }

    IWDG->KR = 0x0000CCCCU; /* start only after PR/RLR are configured */
    IWDG->KR = 0x0000AAAAU; /* refresh */
    return 0;
}

void F411_IWDG_Feed(void)
{
    IWDG->KR = 0x0000AAAAU;
}

uint8_t F411_IWDG_Ready(void)
{
    return (IWDG->SR == 0U) ? 1U : 0U;
}
