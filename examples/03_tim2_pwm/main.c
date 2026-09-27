
#include "f411_rcc.h"
#include "f411_pwm_tim2.h"

int main(void)
{
    F411_RCC_UseHSI16();
    /* PA0 AF1 = TIM2_CH1 on STM32F411. */
    F411_TIM2_PWM_Init(1U, GPIOA, 0U, 1U, 16000000U, 20000U);
    F411_TIM2_PWM_SetDutyPermille(1U, 500U);

    while (1) {}
}
