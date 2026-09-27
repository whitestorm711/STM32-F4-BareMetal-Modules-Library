
#ifndef F411_PWM_TIM2_H
#define F411_PWM_TIM2_H

#include "f411_platform.h"

int F411_TIM2_PWM_Init(uint8_t channel,
                       GPIO_TypeDef *port,
                       uint8_t pin,
                       uint8_t alternate_function,
                       uint32_t timer_clock_hz,
                       uint32_t pwm_hz);
void F411_TIM2_PWM_SetDutyPermille(uint8_t channel, uint16_t permille);
void F411_TIM2_PWM_SetDutyRaw(uint8_t channel, uint32_t compare_value);
uint32_t F411_TIM2_PWM_Period(void);

#endif
