
#include "f411_motor.h"
#include "f411_gpio.h"

static void write_ccr(TIM_TypeDef *timer, uint8_t channel, uint32_t value)
{
    if (channel == 1U) timer->CCR1 = value;
    else if (channel == 2U) timer->CCR2 = value;
    else if (channel == 3U) timer->CCR3 = value;
    else if (channel == 4U) timer->CCR4 = value;
}

void F411_Motor_Init(F411_Motor *motor)
{
    if (motor == NULL || motor->timer == NULL) return;
    F411_GPIO_ConfigOutput(motor->dir_port, motor->dir_pin, 2U);
    if (motor->has_enable) F411_GPIO_ConfigOutput(motor->enable_port, motor->enable_pin, 2U);
    F411_Motor_Stop(motor);
    if (motor->has_enable) F411_Motor_Enable(motor, 0U);
}

void F411_Motor_Enable(F411_Motor *motor, uint8_t enable)
{
    if (motor == NULL || !motor->has_enable) return;
    F411_GPIO_Write(motor->enable_port, motor->enable_pin, enable ? 1U : 0U);
}

void F411_Motor_SetSpeed(F411_Motor *motor, int16_t speed_permille)
{
    uint32_t magnitude;
    uint8_t direction;
    uint64_t period;

    if (motor == NULL || motor->timer == NULL) return;
    if (speed_permille > 1000) speed_permille = 1000;
    if (speed_permille < -1000) speed_permille = -1000;

    direction = (uint8_t)((speed_permille < 0) ? 0U : 1U);
    if (motor->invert_direction) direction ^= 1U;
    F411_GPIO_Write(motor->dir_port, motor->dir_pin, direction);

    magnitude = (speed_permille < 0) ? (uint32_t)(-speed_permille) : (uint32_t)speed_permille;
    period = (uint64_t)motor->timer->ARR + 1ULL;
    write_ccr(motor->timer, motor->pwm_channel, (uint32_t)((period * magnitude) / 1000ULL));
}

void F411_Motor_Stop(F411_Motor *motor)
{
    if (motor == NULL || motor->timer == NULL) return;
    write_ccr(motor->timer, motor->pwm_channel, 0U);
}
