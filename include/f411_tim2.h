
#ifndef F411_TIM2_H
#define F411_TIM2_H

#include "f411_platform.h"

typedef void (*F411_TIM2_Callback)(void);

int F411_TIM2_InitRaw(uint16_t prescaler, uint32_t auto_reload);
int F411_TIM2_InitHz(uint32_t timer_clock_hz, uint32_t update_hz);
void F411_TIM2_Start(void);
void F411_TIM2_Stop(void);
void F411_TIM2_SetCallback(F411_TIM2_Callback callback);
void F411_TIM2_EnableUpdateInterrupt(void);
void F411_TIM2_DisableUpdateInterrupt(void);
uint32_t F411_TIM2_Counter(void);

#endif
