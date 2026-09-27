
#include "f411_rcc.h"

void F411_RCC_EnableGPIO(GPIO_TypeDef *gpio)
{
    if (gpio == GPIOA) RCC->AHB1ENR |= (1U << 0);
    else if (gpio == GPIOB) RCC->AHB1ENR |= (1U << 1);
    else if (gpio == GPIOC) RCC->AHB1ENR |= (1U << 2);
    else if (gpio == GPIOD) RCC->AHB1ENR |= (1U << 3);
}

void F411_RCC_EnableDMA1(void) { RCC->AHB1ENR |= (1U << 21); }
void F411_RCC_EnableDMA2(void) { RCC->AHB1ENR |= (1U << 22); }
void F411_RCC_EnableCRC(void) { RCC->AHB1ENR |= (1U << 12); }
void F411_RCC_EnableTIM1(void) { RCC->APB2ENR |= (1U << 0); }
void F411_RCC_EnableTIM2(void) { RCC->APB1ENR |= (1U << 0); }
void F411_RCC_EnableUSART2(void) { RCC->APB1ENR |= (1U << 17); }
void F411_RCC_EnableADC1(void) { RCC->APB2ENR |= (1U << 8); }
void F411_RCC_EnableSPI1(void) { RCC->APB2ENR |= (1U << 12); }
void F411_RCC_EnableI2C1(void) { RCC->APB1ENR |= (1U << 21); }
void F411_RCC_EnableSYSCFG(void) { RCC->APB2ENR |= (1U << 14); }

int F411_RCC_UseHSI16(void)
{
    RCC->CR |= (1U << 0);
    while ((RCC->CR & (1U << 1)) == 0U) {}
    RCC->CFGR &= ~(3U << 0);
    while (((RCC->CFGR >> 2) & 3U) != 0U) {}
    return 0;
}

static uint32_t timer_clock_hz(uint32_t hclk_hz, uint32_t pclk_hz, uint32_t ppre_shift)
{
    uint32_t ppre = (RCC->CFGR >> ppre_shift) & 7U;
    uint8_t timpre = (uint8_t)((RCC->DCKCFGR >> 24) & 1U);

    if (timpre == 0U)
    {
        return (ppre < 4U) ? pclk_hz : (pclk_hz * 2U);
    }

    /* TIMPRE=1: /1 or /2 APB prescaling gives HCLK; larger division gives
     * 4*PCLK. The divisors encoded by PPRE are 1,2,4,8,16. */
    if (ppre <= 4U) return hclk_hz;
    return pclk_hz * 4U;
}

uint32_t F411_RCC_APB1TimerClockHz(uint32_t hclk_hz, uint32_t pclk1_hz)
{
    return timer_clock_hz(hclk_hz, pclk1_hz, 10U);
}

uint32_t F411_RCC_APB2TimerClockHz(uint32_t hclk_hz, uint32_t pclk2_hz)
{
    return timer_clock_hz(hclk_hz, pclk2_hz, 13U);
}
