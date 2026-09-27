
#include "f411_exti.h"
#include "f411_rcc.h"

static F411_EXTI_Callback g_callbacks[16];

static int valid_line(uint8_t line) { return line < 16U; }

static uint32_t irq_number_for_line(uint8_t line)
{
    if (line <= 4U) return 6U + line;
    if (line <= 9U) return 23U;
    return 40U;
}

static void enable_nvic_irq(uint32_t irq)
{
    if (irq < 32U) NVIC->ISER[0] = (1U << irq);
    else NVIC->ISER[1] = (1U << (irq - 32U));
}

int F411_EXTI_Init(uint8_t line, uint8_t port_code, uint8_t rising, uint8_t falling)
{
    uint32_t index;
    uint32_t shift;
    uint32_t mask;

    if (!valid_line(line) || port_code > 3U) return -1;

    F411_RCC_EnableSYSCFG();

    index = line / 4U;
    shift = (uint32_t)(line % 4U) * 4U;
    mask = 1U << line;

    SYSCFG->EXTICR[index] &= ~(0xFU << shift);
    SYSCFG->EXTICR[index] |= ((uint32_t)port_code << shift);

    EXTI->IMR &= ~mask;
    EXTI->RTSR &= ~mask;
    EXTI->FTSR &= ~mask;
    if (rising) EXTI->RTSR |= mask;
    if (falling) EXTI->FTSR |= mask;

    EXTI->PR = mask;
    EXTI->IMR |= mask;
    enable_nvic_irq(irq_number_for_line(line));
    return 0;
}

void F411_EXTI_SetCallback(uint8_t line, F411_EXTI_Callback callback)
{
    if (valid_line(line)) g_callbacks[line] = callback;
}

void F411_EXTI_Clear(uint8_t line)
{
    if (valid_line(line)) EXTI->PR = (1U << line);
}

uint8_t F411_EXTI_IsPending(uint8_t line)
{
    if (!valid_line(line)) return 0U;
    return (uint8_t)((EXTI->PR >> line) & 1U);
}

void F411_EXTI_Disable(uint8_t line)
{
    if (!valid_line(line)) return;
    EXTI->IMR &= ~(1U << line);
    EXTI->PR = (1U << line);
}

static void dispatch(uint8_t first, uint8_t last)
{
    uint32_t pending = EXTI->PR;
    for (uint8_t line = first; line <= last; line++)
    {
        uint32_t mask = 1U << line;
        if ((pending & mask) != 0U)
        {
            EXTI->PR = mask;
            if (g_callbacks[line] != NULL) g_callbacks[line](line);
        }
    }
}

void EXTI0_IRQHandler(void) { dispatch(0U, 0U); }
void EXTI1_IRQHandler(void) { dispatch(1U, 1U); }
void EXTI2_IRQHandler(void) { dispatch(2U, 2U); }
void EXTI3_IRQHandler(void) { dispatch(3U, 3U); }
void EXTI4_IRQHandler(void) { dispatch(4U, 4U); }
void EXTI9_5_IRQHandler(void) { dispatch(5U, 9U); }
void EXTI15_10_IRQHandler(void) { dispatch(10U, 15U); }
