#include "f411_pwm_tim2.h"
#include "f411_rcc.h"
#include "f411_gpio.h"

static int channel_valid(uint8_t ch) { return ch >= 1U && ch <= 4U; }
static uint32_t ccer_shift(uint8_t ch) { return ((uint32_t)ch - 1U) * 4U; }

static void configure_channel_output(uint8_t channel)
{
    if (channel == 1U)
    {
        TIM2->CCMR1 &= ~((3U << 0) | (1U << 2) | (1U << 3) | (7U << 4));
        TIM2->CCMR1 |= (1U << 3) | (6U << 4);
    }
    else if (channel == 2U)
    {
        TIM2->CCMR1 &= ~((3U << 8) | (1U << 10) | (1U << 11) | (7U << 12));
        TIM2->CCMR1 |= (1U << 11) | (6U << 12);
    }
    else if (channel == 3U)
    {
        TIM2->CCMR2 &= ~((3U << 0) | (1U << 2) | (1U << 3) | (7U << 4));
        TIM2->CCMR2 |= (1U << 3) | (6U << 4);
    }
    else
    {
        TIM2->CCMR2 &= ~((3U << 8) | (1U << 10) | (1U << 11) | (7U << 12));
        TIM2->CCMR2 |= (1U << 11) | (6U << 12);
    }
}

static void write_ccr(uint8_t channel, uint32_t value)
{
    if (channel == 1U) TIM2->CCR1 = value;
    else if (channel == 2U) TIM2->CCR2 = value;
    else if (channel == 3U) TIM2->CCR3 = value;
    else if (channel == 4U) TIM2->CCR4 = value;
}

int F411_TIM2_PWM_Init(uint8_t channel,
                       GPIO_TypeDef *port,
                       uint8_t pin,
                       uint8_t alternate_function,
                       uint32_t timer_clock_hz,
                       uint32_t pwm_hz)
{
    uint64_t period_counts;

    if (!channel_valid(channel) || port == NULL || pin > 15U || timer_clock_hz == 0U || pwm_hz == 0U) return -1;
    period_counts = timer_clock_hz / pwm_hz;
    if (period_counts < 1ULL || period_counts > 0x100000000ULL) return -2;

    F411_RCC_EnableTIM2();
    F411_GPIO_ConfigAF(port, pin, alternate_function, 3U, 0U);

    TIM2->CR1 &= ~(1U << 0);
    TIM2->PSC = 0U;
    TIM2->ARR = (uint32_t)(period_counts - 1ULL);
    TIM2->CR1 |= (1U << 7);

    TIM2->CCER &= ~(1U << ccer_shift(channel));
    configure_channel_output(channel);
    write_ccr(channel, 0U);

    TIM2->EGR = (1U << 0);
    TIM2->CCER |= (1U << ccer_shift(channel));
    TIM2->CR1 |= (1U << 0);
    return 0;
}

void F411_TIM2_PWM_SetDutyRaw(uint8_t channel, uint32_t compare_value)
{
    uint64_t max_compare;
    if (!channel_valid(channel)) return;
    max_compare = (uint64_t)TIM2->ARR + 1ULL;
    if ((uint64_t)compare_value > max_compare) compare_value = (uint32_t)max_compare;
    write_ccr(channel, compare_value);
}

void F411_TIM2_PWM_SetDutyPermille(uint8_t channel, uint16_t permille)
{
    uint64_t period_counts;
    uint32_t compare;
    if (!channel_valid(channel)) return;
    if (permille > 1000U) permille = 1000U;

    period_counts = (uint64_t)TIM2->ARR + 1ULL;
    compare = (uint32_t)((period_counts * permille) / 1000ULL);
    if (compare > TIM2->ARR) compare = TIM2->ARR;
    write_ccr(channel, compare);
}

uint32_t F411_TIM2_PWM_Period(void)
{
    return TIM2->ARR + 1U;
}
