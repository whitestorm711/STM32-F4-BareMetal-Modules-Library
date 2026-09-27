
#include "f411_tim2.h"
#include "f411_rcc.h"

static F411_TIM2_Callback g_callback;

int F411_TIM2_InitRaw(uint16_t prescaler, uint32_t auto_reload)
{
    F411_RCC_EnableTIM2();

    TIM2->CR1 = 0U;
    TIM2->CR2 = 0U;
    TIM2->SMCR = 0U;
    TIM2->DIER = 0U;
    TIM2->CCER = 0U;
    TIM2->PSC = prescaler;
    TIM2->ARR = auto_reload;
    TIM2->CNT = 0U;
    TIM2->EGR = (1U << 0); /* UG */
    TIM2->SR &= ~(1U << 0);
    return 0;
}

int F411_TIM2_InitHz(uint32_t timer_clock_hz, uint32_t update_hz)
{
    uint64_t total_counts;
    uint32_t psc = 0U;
    uint64_t max_counts = 0x100000000ULL;
    uint32_t arr;

    if (timer_clock_hz == 0U || update_hz == 0U) return -1;
    total_counts = timer_clock_hz / update_hz;
    if (total_counts == 0U) return -2;

    if (total_counts > max_counts)
    {
        uint64_t divisor = (total_counts + max_counts - 1ULL) / max_counts;
        if (divisor > 65536ULL) return -3;
        psc = (uint32_t)(divisor - 1ULL);
        total_counts = timer_clock_hz / ((uint64_t)(psc + 1U) * update_hz);
    }

    if (total_counts == 0ULL || total_counts > max_counts) return -4;
    arr = (uint32_t)(total_counts - 1ULL);
    return F411_TIM2_InitRaw((uint16_t)psc, arr);
}

void F411_TIM2_Start(void)
{
    TIM2->CR1 |= (1U << 0);
}

void F411_TIM2_Stop(void)
{
    TIM2->CR1 &= ~(1U << 0);
}

void F411_TIM2_SetCallback(F411_TIM2_Callback callback)
{
    g_callback = callback;
}

void F411_TIM2_EnableUpdateInterrupt(void)
{
    TIM2->DIER |= (1U << 0); /* UIE */
    /* TIM2 IRQ = 28 -> ISER[0], bit 28. */
    NVIC->ISER[0] = (1U << 28);
}

void F411_TIM2_DisableUpdateInterrupt(void)
{
    TIM2->DIER &= ~(1U << 0);
}

uint32_t F411_TIM2_Counter(void)
{
    return TIM2->CNT;
}

void TIM2_IRQHandler(void)
{
    if ((TIM2->SR & (1U << 0)) != 0U)
    {
        TIM2->SR &= ~(1U << 0);
        if (g_callback != NULL) g_callback();
    }
}
