
#include "f411_tim1_3phase.h"
#include "f411_rcc.h"
#include "f411_gpio.h"

static uint8_t clamp_ckd(uint8_t ckd_div)
{
    if (ckd_div <= 1U) return 1U;
    if (ckd_div <= 2U) return 2U;
    return 4U;
}

static uint64_t deadtime_ns_for_code(uint32_t timer_clock_hz, uint8_t ckd_div, uint8_t code)
{
    uint64_t t_dts_ps;
    uint64_t ticks;
    uint32_t div = clamp_ckd(ckd_div);

    if (timer_clock_hz == 0U) return 0ULL;
    /* t_DTS in picoseconds, rounded down. */
    t_dts_ps = (1000000000000ULL * (uint64_t)div) / (uint64_t)timer_clock_hz;

    if ((code & 0xE0U) == 0x00U)
    {
        ticks = code;
    }
    else if ((code & 0xE0U) == 0x80U)
    {
        ticks = (uint64_t)(64U + (code & 0x3FU)) * 2ULL;
    }
    else if ((code & 0xE0U) == 0xC0U)
    {
        ticks = (uint64_t)(32U + (code & 0x1FU)) * 8ULL;
    }
    else
    {
        ticks = (uint64_t)(32U + (code & 0x1FU)) * 16ULL;
    }
    return (ticks * t_dts_ps) / 1000ULL;
}

uint8_t F411_TIM1_EncodeDeadtime(uint32_t timer_clock_hz, uint8_t ckd_div, uint32_t deadtime_ns)
{
    uint8_t best = 0U;
    uint64_t best_err = UINT64_MAX;
    for (uint32_t code = 0U; code < 256U; code++)
    {
        uint64_t actual = deadtime_ns_for_code(timer_clock_hz, ckd_div, (uint8_t)code);
        uint64_t err = (actual > deadtime_ns) ? (actual - deadtime_ns) : ((uint64_t)deadtime_ns - actual);
        if (err < best_err)
        {
            best_err = err;
            best = (uint8_t)code;
        }
    }
    return best;
}

static void set_ccr(uint8_t ch, uint32_t value)
{
    if (ch == 1U) TIM1->CCR1 = value;
    else if (ch == 2U) TIM1->CCR2 = value;
    else if (ch == 3U) TIM1->CCR3 = value;
}

int F411_TIM1_3Phase_Init(uint32_t timer_clock_hz,
                          uint32_t pwm_hz,
                          uint16_t prescaler,
                          uint8_t deadtime_dtg)
{
    uint64_t period_counts;
    if (timer_clock_hz == 0U || pwm_hz == 0U) return -1;
    period_counts = timer_clock_hz / ((uint32_t)prescaler + 1U) / pwm_hz;
    /* TIM1 is a 16-bit advanced-control timer on STM32F411. */
    if (period_counts < 1ULL || period_counts > 0x10000ULL) return -2;

    F411_RCC_EnableTIM1();
    F411_GPIO_ConfigAF(GPIOA, 8U, 1U, 3U, 0U);
    F411_GPIO_ConfigAF(GPIOA, 9U, 1U, 3U, 0U);
    F411_GPIO_ConfigAF(GPIOA, 10U, 1U, 3U, 0U);
    F411_GPIO_ConfigAF(GPIOB, 13U, 1U, 3U, 0U);
    F411_GPIO_ConfigAF(GPIOB, 14U, 1U, 3U, 0U);
    F411_GPIO_ConfigAF(GPIOB, 15U, 1U, 3U, 0U);

    TIM1->CR1 = 0U;
    TIM1->CR2 = 0U;
    TIM1->SMCR = 0U;
    TIM1->DIER = 0U;
    TIM1->CCER = 0U;
    TIM1->PSC = prescaler;
    TIM1->ARR = (uint32_t)(period_counts - 1ULL);
    TIM1->RCR = 0U;
    TIM1->CR1 |= (1U << 7); /* ARPE */

    TIM1->CCMR1 = 0U;
    TIM1->CCMR2 = 0U;
    TIM1->CCMR1 |= (1U << 3) | (6U << 4);   /* CH1 PWM1 preload */
    TIM1->CCMR1 |= (1U << 11) | (6U << 12); /* CH2 PWM1 preload */
    TIM1->CCMR2 |= (1U << 3) | (6U << 4);   /* CH3 PWM1 preload */

    TIM1->CCR1 = 0U;
    TIM1->CCR2 = 0U;
    TIM1->CCR3 = 0U;

    TIM1->CCER |= (1U << 0) | (1U << 2);
    TIM1->CCER |= (1U << 4) | (1U << 6);
    TIM1->CCER |= (1U << 8) | (1U << 10);

    /* BDTR fields verified against RM0383: DTG 7:0, OSSI 10, OSSR 11,
     * BKE 12, BKP 13, AOE 14, MOE 15. Break input is left disabled here. */
    TIM1->BDTR = (uint32_t)deadtime_dtg;
    TIM1->BDTR |= (1U << 10) | (1U << 11); /* deterministic off/run states */

    TIM1->EGR = (1U << 0);
    TIM1->BDTR |= (1U << 15); /* MOE */
    TIM1->CR1 |= (1U << 0);   /* CEN */
    return 0;
}

void F411_TIM1_3Phase_SetDutyPermille(uint16_t phase1, uint16_t phase2, uint16_t phase3)
{
    uint64_t period = (uint64_t)TIM1->ARR + 1ULL;
    if (phase1 > 1000U) phase1 = 1000U;
    if (phase2 > 1000U) phase2 = 1000U;
    if (phase3 > 1000U) phase3 = 1000U;
    uint32_t max_compare = TIM1->ARR;
    uint32_t c1 = (uint32_t)((period * phase1) / 1000ULL);
    uint32_t c2 = (uint32_t)((period * phase2) / 1000ULL);
    uint32_t c3 = (uint32_t)((period * phase3) / 1000ULL);
    if (c1 > max_compare) c1 = max_compare;
    if (c2 > max_compare) c2 = max_compare;
    if (c3 > max_compare) c3 = max_compare;
    set_ccr(1U, c1);
    set_ccr(2U, c2);
    set_ccr(3U, c3);
}

void F411_TIM1_3Phase_EnableOutputs(void)
{
    TIM1->BDTR |= (1U << 15);
}

void F411_TIM1_3Phase_DisableOutputs(void)
{
    TIM1->BDTR &= ~(1U << 15);
}

void F411_TIM1_3Phase_Stop(void)
{
    TIM1->CR1 &= ~(1U << 0);
    TIM1->BDTR &= ~(1U << 15);
}
