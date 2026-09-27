
#include "f411_systick.h"

static volatile uint32_t g_f411_ms;

int F411_SysTick_Init(uint32_t core_clock_hz)
{
    uint32_t reload;
    if (core_clock_hz < 1000U) return -1;

    reload = core_clock_hz / 1000U;
    if (reload == 0U || reload > 0x01000000U) return -2;

    SysTick->CTRL = 0U;
    SysTick->LOAD = reload - 1U;
    SysTick->VAL = 0U;
    SysTick->CTRL = (1U << 2) | (1U << 1) | (1U << 0);
    g_f411_ms = 0U;
    return 0;
}

uint32_t F411_SysTick_Millis(void)
{
    return g_f411_ms;
}

void F411_SysTick_DelayMs(uint32_t ms)
{
    uint32_t start = g_f411_ms;
    while ((uint32_t)(g_f411_ms - start) < ms) {}
}

void SysTick_Handler(void)
{
    g_f411_ms++;
}
