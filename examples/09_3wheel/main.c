
#include "f411_pwm_tim2.h"
#include "f411_motor.h"
#include "f411_drv3wheel.h"
#include "f411_rcc.h"

int main(void)
{
    static F411_Motor m0 = {TIM2, 1U, GPIOB, 0U, NULL, 0U, 0U, 0U};
    static F411_Motor m1 = {TIM2, 2U, GPIOB, 1U, NULL, 0U, 0U, 0U};
    static F411_Motor m2 = {TIM2, 3U, GPIOB, 2U, NULL, 0U, 0U, 0U};
    static F411_Drive3Wheel drive;

    F411_RCC_UseHSI16();
    F411_TIM2_PWM_Init(1U, GPIOA, 0U, 1U, 16000000U, 20000U);
    F411_TIM2_PWM_Init(2U, GPIOA, 1U, 1U, 16000000U, 20000U);
    F411_TIM2_PWM_Init(3U, GPIOA, 2U, 1U, 16000000U, 20000U);

    F411_Motor_Init(&m0);
    F411_Motor_Init(&m1);
    F411_Motor_Init(&m2);
    F411_Drive3Wheel_Init(&drive, &m0, &m1, &m2);

    F411_Drive3Wheel_SetWheelSpeeds(&drive, 300, -300, 300);
    while (1) {}
}
