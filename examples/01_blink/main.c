
#include "f411_rcc.h"
#include "f411_gpio.h"
#include "f411_systick.h"

int main(void)
{
    F411_RCC_UseHSI16();
    F411_SysTick_Init(16000000U);
    F411_GPIO_ConfigOutput(GPIOC, 13U, 2U);

    while (1)
    {
        F411_GPIO_Toggle(GPIOC, 13U);
        F411_SysTick_DelayMs(500U);
    }
}
