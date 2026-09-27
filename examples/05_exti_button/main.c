
#include "f411_gpio.h"
#include "f411_exti.h"
#include "f411_rcc.h"

static volatile uint32_t press_count;
static void on_exti(uint8_t line)
{
    (void)line;
    press_count++;
}

int main(void)
{
    F411_RCC_UseHSI16();
    F411_GPIO_ConfigInput(GPIOC, 13U, F411_GPIO_PULL_UP);
    F411_EXTI_SetCallback(13U, on_exti);
    F411_EXTI_Init(13U, F411_GPIO_PORT_C, 0U, 1U);

    while (1) {}
}
