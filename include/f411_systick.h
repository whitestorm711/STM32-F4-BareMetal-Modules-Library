
#ifndef F411_SYSTICK_H
#define F411_SYSTICK_H

#include "f411_platform.h"

int F411_SysTick_Init(uint32_t core_clock_hz);
uint32_t F411_SysTick_Millis(void);
void F411_SysTick_DelayMs(uint32_t ms);

#endif
