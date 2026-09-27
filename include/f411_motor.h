
#ifndef F411_MOTOR_H
#define F411_MOTOR_H

#include "f411_platform.h"

typedef struct
{
    TIM_TypeDef *timer;
    uint8_t pwm_channel;
    GPIO_TypeDef *dir_port;
    uint8_t dir_pin;
    GPIO_TypeDef *enable_port;
    uint8_t enable_pin;
    uint8_t has_enable;
    uint8_t invert_direction;
} F411_Motor;

void F411_Motor_Init(F411_Motor *motor);
void F411_Motor_Enable(F411_Motor *motor, uint8_t enable);
void F411_Motor_SetSpeed(F411_Motor *motor, int16_t speed_permille);
void F411_Motor_Stop(F411_Motor *motor);

#endif
