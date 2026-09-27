
#ifndef F411_EXTI_H
#define F411_EXTI_H

#include "f411_platform.h"

typedef void (*F411_EXTI_Callback)(uint8_t line);

enum
{
    F411_GPIO_PORT_A = 0U,
    F411_GPIO_PORT_B = 1U,
    F411_GPIO_PORT_C = 2U,
    F411_GPIO_PORT_D = 3U
};

int F411_EXTI_Init(uint8_t line, uint8_t port_code, uint8_t rising, uint8_t falling);
void F411_EXTI_SetCallback(uint8_t line, F411_EXTI_Callback callback);
void F411_EXTI_Clear(uint8_t line);
uint8_t F411_EXTI_IsPending(uint8_t line);
void F411_EXTI_Disable(uint8_t line);

#endif
